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
		perror(strerror(errno));
		log("ERROR: " + errorMessage);
		exit(1);
	}
}

namespace http
{;

			// CONSTRUCTOR
TcpServer::TcpServer(class Configuration configuration)
	: _config(configuration), 
	_nbListeningSockets(0)
{
	setUpListeningSockets();
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
	return 0;
}

void	TcpServer::setUpListeningSockets()
{
	t_socket		listening_socket;
	struct pollfd	listener;
	int				re_use = 1;

	/*************************************************************/
	/* Moet loopen door het aantal poorten uit config            */
	/* setup van de struct s_socket (socket info)   			 */
	/*************************************************************/
	for (int i = 0; i < 1; i++) {								
		memset(&listening_socket, 0, sizeof(listening_socket));
		listening_socket.socket_info.sin_family = AF_INET;
		// listening_socket.socket_info.sin_port = htons(_config.getPort());
							int test_port = 8000;							// tmp
		listening_socket.socket_info.sin_port = htons(test_port++); 			// tmp
		listening_socket.socket_info.sin_addr.s_addr = inet_addr(_config.getIP().c_str());
		
		listening_socket.socket_address_len = sizeof(listening_socket.socket_info);
		_socketInfo.push_back(listening_socket);

		/*************************************************************/
		/* setup van de pollfd struct					            */
		/* setsocktopt() zorgt ervoor dat we ingeval van een restart
			de socket kunnen hergebruiken                            */

		/*************************************************************/
		memset(&listener, 0, sizeof(listener));
		listener.fd = socket(AF_INET, SOCK_STREAM, 0);		
		if (listener.fd < 0) {
			exitWithError("Cannot create listening socket");
		}
		if (setsockopt(listener.fd, SOL_SOCKET, SO_REUSEPORT, &re_use, sizeof(re_use)) < 0) {
			exitWithError("setsockopt error\n");
		}	
		fcntl(listener.fd, F_SETFL, O_NONBLOCK);			
		listener.events = POLLIN;
		if (bind(listener.fd, (sockaddr *)&_socketInfo[i].socket_info, _socketInfo[i].socket_address_len) < 0) {		/// we casten van sockaddr_in -> sock_addr waarom?
			exitWithError("Cannot bind() socket to address");
		}
		_pollFds.push_back(listener);
		_nbListeningSockets++;
	}
}

void TcpServer::startListen()
{
	int	poll_count;
	
	for (int i = 0; i < _nbListeningSockets; i++) {
		if (listen(_pollFds[i].fd, QUEU_LIMIT_LISTEN) < 0)			
			exitWithError("Socket listen failed");
	}
	// logStartupMessage(_socketAddress);
	while (_isServerRunning) {
		log("\n====== Waiting for a new connection ======\n");
		sleep(1);


		/****************************************************************/
		/* 	Poll checkt elke socket of er iets gebeurt (read / 			*/		
		/*	write request) en houdt dit bij in de array van structs 	*/
		/*	_pollFds (door .events aan te passen) 					*/
		/*	De -1 geeft aan dat hij oneindig lang wacht op read/write 	*/
		/*	requests)   												*/
		/****************************************************************/
		poll_count = poll (&_pollFds[0], _pollFds.size(), -1);		
		if (poll_count == -1) {
			exitWithError("Poll count = negative (TcpServer::startListen()");
		}
		lookupActiveSocket();
	}	
}

// Finds the socket where poll() found activity 
void	TcpServer::lookupActiveSocket()
{
	int i = 0;

	for (; i < _nbListeningSockets; i++)
	{
		if (_pollFds[i].revents == 0)			// nothing happened on this socket
			continue ;
		else
			newConnection(i);	
	}


	/****************************************************************/
	/* 	loop door de client sockets om te kijken welke actief is	*/
	/*	POLLIN geeft aan dat een read request klaar staat			*/
	/*	POLLOUT geeft aan dat een write request klaar staat			*/
	/****************************************************************/
	for (; i < (_pollFds.size() - _nbListeningSockets + 1); i++) {
		if (_pollFds[i].revents == 0)
			continue;
		if (_pollFds[i].revents & POLLIN) {
			printf("POLLIN with i = %i\n", i);		
			receiveRequest(i);
		} else if (_pollFds[i].revents & POLLOUT) {

			printf("POLLOUT with i = %i\n", i);		

			sendResponse(i);
		}
	}  													 		
}


// 	Accepts new connections. These don't need to be set to non-blocking since they inherit from the listeningsocket
// 	(which is already non-blocking)
void TcpServer::newConnection(int active_socket_idx)
{
	// Client	*new_pollfd = new Client(_pollFds.size() + 1);
	struct pollfd	new_pollfd;
	t_socket		new_socket;

	memset(&new_pollfd, 0, sizeof(struct pollfd));
	memset(&new_socket, 0, sizeof(t_socket));
	new_pollfd.fd = accept(_pollFds[active_socket_idx].fd, (sockaddr *)&new_socket.socket_info, &new_socket.socket_address_len);
	if (new_pollfd.fd == -1) {
		exitWithError("ERROR: accept() (TcpServer::newConnection");
	}
															

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
	new_pollfd.events = POLLIN;
	_pollFds.push_back(new_pollfd);
	_socketInfo.push_back(new_socket);

	std::cout << "Server accepted incoming connection from ADDRESS: "
			<< inet_ntoa(new_socket.socket_info.sin_addr) << "; PORT: " 
			<< ntohs(new_socket.socket_info.sin_port) << "; Socket fd: " << _pollFds.back().fd << std::endl;
}

void	TcpServer::closeClientConnection(int close)
{
											////////////////////////// remove both _socketInfo en _pollFDs!!!!!

}



// idx geeft aan welke socket in de vector een POLLIN (read activity) heeft
void TcpServer::receiveRequest(int idx)
{
	char	buff[BUFFER_SIZE];
	size_t	bytes_received;

	bytes_received = recv(_pollFds[idx].fd, buff, sizeof(buff), 0);		// only difference between recv and read is dat recv als laatste argument flags heeft (flag == null, dan is recv == read)
																			// evt kan je met deze flag bepaalde errors opvangen die in de socket gebeuren 
	if (bytes_received <= 0) {
		if (bytes_received == 0)
			std::cout << "Socket fd " << _pollFds[idx].fd << " closed their connection." << std::endl;
		else
			std::cout << "Recv() error on socket fd " << _pollFds[idx].fd << " in TcpServer::startListen()" << std::endl;		
		close (_pollFds[idx].fd);
		_socketInfo.erase(_socketInfo.begin() + idx);
		_pollFds.erase(_pollFds.begin() + idx);  // needs testing, en: moet een socket closen als hij een recv error geeft?
	} 
		
	_pollFds[idx].events = POLLIN | POLLOUT;			// Socket heeft data gestuurd en kan dus nu op POLLIN | POLLOUT zodat hij data kan ontvangen

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
	if (_serverMessage.empty())								// Deze check doen waar de _serverMessage wordt gemaakt?
		std::cout << "No server message! Client won't receive anything!" << std::endl;

	_unsendServerMessage = _serverMessage;

	while (!_unsendServerMessage.empty())
	{
		bytes_send = send(_pollFds[idx].fd, &_unsendServerMessage, _unsendServerMessage.size(), 0);
		if (bytes_send < 0)
			std::cout << "Send error in TcpServer::sendResponse()" << std::endl;
		_unsendServerMessage.erase(0, bytes_send);
	}
}



void TcpServer::closeServer()
{
	// close(_pollFds[0].fd); 	// exit closes all fds(?)
	exit(0);
								/*
									Als we threads gaan gebruiken geen exit() meer.
									Exit() sluit dan automatisch alle threads, maar dat is
									c++11 functionaliteit. Dus in dat geval zouden we zelf 
									threads moeten sluiten en dan exit oproepen
								*/
}




} // namespace http
