#include "TcpServer.hpp"
#include "BuildResponse.hpp"

#include <errno.h>
#include <poll.h>
#include <fcntl.h>
#include <iostream>
#include <sstream>
#include <unistd.h>
#include <fstream>

#define QUEU_LIMIT_LISTEN 20

namespace
{;
	const int BUFFER_SIZE = 30720;

	void log(const std::string &message)
	{
		std::cout << message << std::endl;
	}

	void logStartupMessage(struct sockaddr_in _socketAddress[0])
	{
		std::ostringstream ss;
		ss	<< "\n*** Listening on ADDRESS: "
			<< inet_ntoa(_socketAddress[0].sin_addr) 
			<< " PORT: "
			<< ntohs(_socketAddress[0].sin_port) 
			<< " ***\n\n";
		log(ss.str());
	}

	void exitWithError(const std::string &errorMessage)
	{
		log("ERROR: " + errorMessage);
		exit(1);
	}
}

namespace http
{;
			// CONSTRUCTOR
TcpServer::TcpServer(class Configuration configuration)
	: _config(configuration), 
	
	_socket_fds(), _number_of_socket_fds(0)
{
	struct sockaddr_in	socket_in;

	socket_in.sin_family = AF_INET;
	socket_in.sin_addr.s_addr = inet_addr(_config.getIP().c_str());
	// _socketAddress.sin_port = htons(_config.getPort());
	socket_in.sin_port = htons(port++); 

	_socketAddress.pushback(socket_in);


					////////////////////////////////////////////////// ############################//////////////////////////////////
					/*
					
								HIER BEN IK. _socketAddress en _socketAddress_len zijn vectors, dit meot nog verder worden verwerkt in de code
								zodat uiteindelijk tijdens het loopen door listening sockets de juiste waardes van deze twee attributes kunnenn worden
								ingelezen
					
					
					*/


	if (startServer() != 0) {
		std::ostringstream ss;
		ss << "Failed to start server with PORT: " << ntohs(_socketAddress.sin_port);
		log(ss.str());
	}
		
	_isServerRunning = true;
	startListen();
}

			// DESTRUCTOR
TcpServer::~TcpServer()
{
	closeServer();
}

			// PRIVATE FUNCTIONS
int TcpServer::startServer()
{
	struct pollfd	listener;
	
	listener.fd = socket(AF_INET, SOCK_STREAM, 0);		
	if (listener.fd < 0) {
		exitWithError("Cannot create listening socket");
		return 1;
	}
	fcntl(listener.fd, F_SETFL, O_NONBLOCK);
	if (setsockopt(listener.fd, SOL_SOCKET, SO_REUSEPORT, &reuse, sizeof(reuse)) < 0)
		exitWithError("setsockopt error\n");
	_listening_socket.push_back(listener.fd);
	listener.events = POLLIN;			// listener socket only reports when ready to read on inc connection
	_socket_fds.push_back(listener);
	_number_of_socket_fds++;
	if (bind(listener.fd, (sockaddr *)&_socketAddress, _socketAddress_len) < 0) {
		exitWithError("Cannot connect socket to address");
		return 1;
	}
	return 0;
}

void TcpServer::startListen()
{
	int	poll_count;
	
	for (int i = 0; i < _listening_socket.size(); i++) {
		if (listen(_listening_socket[i], QUEU_LIMIT_LISTEN) < 0)
			exitWithError("Socket listen failed");
	}
	// logStartupMessage(_socketAddress);
	while (_isServerRunning) {
		log("\n====== Waiting for a new connection ======\n");
		

		sleep(1);

								/*
									Poll checkt elke socket of er iets gebeurt (read / write request) en houdt 
									dit bij in de array van structs _socket_fds (door .events aan te passen)
									De -1 geeft aan dat hij oneindig lang wacht op read/write requests)
								*/ 
		poll_count = poll (&_socket_fds[0], _number_of_socket_fds, -1);		
		if (poll_count == -1) {
			exitWithError("Poll count = negative (TcpServer::startListen()");
		}
		lookupActiveSocket();			// vervolgens kunnen we kijken op welke socket er iets is gebeurd
	}	
}

void TcpServer::acceptConnection(int idx) 
{
	struct pollfd	add_to_socket_fd;

	add_to_socket_fd.fd = accept(_listening_socket[idx], (sockaddr *)&_socketAddress, &_socketAddress_len);
	if (add_to_socket_fd.fd == -1)
		exitWithError("ERROR: accept() (TcpServer::acceptConnection");

	fcntl(add_to_socket_fd.fd, F_SETFL, O_NONBLOCK);			// nodig om elke socket op non blocking te zetten?


								/*
									set events for added socket?

									add_to_socket.events = POLLIN | POLLOUT
									hoe weten we of een nieuwe connectie POLLIN, POLLOUT of POLLIN | POLLOUT moet zijn?

									huidige vorm = 
									- nieuwe connectie dan gaat socket op POLLIN
									- een read op een socket ontvangen dan gaat hij op POLLIN | POLLOUT

									Op deze manier zou een connectie dus nooit iets ontvangen voordat hij zelf iets stuurt,
									maar is ook logisch want een connectie moet altijd eerst iets sturen (request) voordat hij kan ontvangen
							
									evt toevoegen : 
									socket kan weer op alleen POLLIN nadat de gehele _serverMessage is verstuurd
									
								*/
	add_to_socket_fd.events = POLLIN;	
	_socket_fds.push_back(add_to_socket_fd);
	_number_of_socket_fds++;
	std::cout << "Server accepted incoming connection from ADDRESS: "
			<< inet_ntoa(_socketAddress.sin_addr) << "; PORT: " 
			<< ntohs(_socketAddress.sin_port) << "; Socket fd: " << _socket_fds.back().fd << std::endl;
}

// Finds the socket where poll() found activity 
void	TcpServer::lookupActiveSocket()
{
	for (int i = 0; i < _number_of_socket_fds; i++) {
		if (_socket_fds[i].revents & POLLIN) {
																// revents houdt bij of er iets gebeurd is (POLLIN == receive / read, POLLOUT == send / write)
			for (int j = 0; i < _listening_socket.size(); i++)													
				if (_socket_fds[i].fd == _listening_socket[j]) 			// als dit listeneing socket is betekent dat dat er een nieuwe connectie is 
					acceptConnection(j);
				else 
					receiveRequest(i);
		} else if (_socket_fds[i].revents & POLLOUT) {
			sendResponse(i);
		}

	}  													 		
}


// idx geeft aan welke socket in de vector een POLLIN (read activity) heeft
void TcpServer::receiveRequest(int idx)
{
	char	buff[BUFFER_SIZE];
	size_t	bytes_received;

	bytes_received = recv(_socket_fds[idx].fd, buff, sizeof(buff), 0);		// only difference between recv and read is dat recv als laatste argument flags heeft (flag == null, dan is recv == read)
																			// evt kan je met deze flag bepaalde errors opvangen die in de socket gebeuren 
	if (bytes_received <= 0) {
		if (bytes_received == 0)
			std::cout << "Socket fd " << _socket_fds[idx].fd << " closed their connection." << std::endl;
		else
			std::cout << "Recv() error on socket fd " << _socket_fds[idx].fd << " in TcpServer::startListen()" << std::endl;		
		close (_socket_fds[idx].fd);
		_socket_fds.erase(_socket_fds.begin() + idx);  // needs testing, en: moet een socket closen als hij een recv error geeft?
		_number_of_socket_fds--;
	} 
		
	_socket_fds[idx].events = POLLIN | POLLOUT;			// Socket heeft data gestuurd en kan dus nu op POLLIN | POLLOUT zodat hij data kan ontvangen

	_request.initHTTPRequest(std::string(buff));
	// if (!_request.isValidMethod())
	// 	exitWithError("Invalid method, only GET, POST and DELETED are supported");
	std::ostringstream ss;
	ss	<< "Received request: Method = " << _request.getMethod()
		<< " URI = " 					 << _request.getURI()
		<< " HTTP Version = " 			 << _request.getHTTPVersion();
	log(ss.str());
}

// idx geeft aan welke socket in de vector een POLLOUT (write activity) heeft
void TcpServer::sendResponse(int idx)
{
	size_t	bytes_send;
	class	BuildResponse respons(_request, _config);

	_serverMessage = respons.getMessage("200 OK");

	// _serverMessage = "Test\n";
	if (_serverMessage.empty())							// Deze check doen waar de _serverMessage wordt gemaakt?
		std::cout << "No server message! Client won't receive anything!" << std::endl;

	_unsendServerMessage = _serverMessage;

	while (!_unsendServerMessage.empty())
	{
		bytes_send = send(_socket_fds[idx].fd, &_unsendServerMessage, _unsendServerMessage.size(), 0);
		if (bytes_send < 0)
			std::cout << "Send error in TcpServer::sendResponse()" << std::endl;
		_unsendServerMessage.erase(0, bytes_send);
	}
}



void TcpServer::closeServer()
{
	// close(_socket_fds[0].fd); 	// exit closes all fds(?)
	exit(0);					
								/*
									Als we threads gaan gebruiken geen exit() meer.
									Exit() sluit dan automatisch alle threads, maar dat is
									c++11 functionaliteit. Dus in dat geval zouden we zelf 
									threads moeten sluiten en dan exit oproepen
								*/
}




} // namespace http
