#include "TcpServer.hpp"
#include "BuildResponse.hpp"

#include <errno.h>
#include <poll.h>
#include <fcntl.h>
#include <iostream>
#include <sstream>
#include <unistd.h>
#include <fstream>

namespace
{;
	const int BUFFER_SIZE = 30720;

	void log(const std::string &message)
	{
		std::cout << message << std::endl;
	}

	void logStartupMessage(struct sockaddr_in _socketAddress)
	{
		std::ostringstream ss;
		ss	<< "\n*** Listening on ADDRESS: "
			<< inet_ntoa(_socketAddress.sin_addr) 
			<< " PORT: "
			<< ntohs(_socketAddress.sin_port) 
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
	: _config(configuration), _socketAddress(), 
	_socketAddress_len(sizeof(_socketAddress)),
	
	
	//// toegevoegd door Jonathan
	 _socket_fds() 
	////
{
	_socketAddress.sin_family = AF_INET;
	_socketAddress.sin_port = htons(_config.getPort());
	_socketAddress.sin_addr.s_addr = inet_addr(_config.getIP().c_str());
	_number_of_socket_fds = 0;
	if (startServer() != 0) {
		std::ostringstream ss;
		ss << "Failed to start server with PORT: " << ntohs(_socketAddress.sin_port);
		log(ss.str());
	}
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
	
	listener.events = POLLIN;			// listener socket only reports when ready to read on inc connection
	_socket_fds.push_back(listener);
	_number_of_socket_fds++;
	if (bind(listener.fd, (sockaddr *)&_socketAddress, _socketAddress_len) < 0) {
		exitWithError("Cannot connect socket to address");
		return 1;
	}
	return 0;
}
void TcpServer::closeServer()
{
	close(_socket_fds[0].fd); // exit closes all fds(?)
	exit(0);
}

// _socket_fds[0] holds listening socket
void TcpServer::startListen()
{
	int	poll_count;
	
	if (listen(_socket_fds[0].fd, 20) < 0)
		exitWithError("Socket listen failed");
	logStartupMessage(_socketAddress);
	while (true) {
		log("\n====== Waiting for a new connection ======\n");
		
		// 	Poll checkt elke socket of er iets gebeurt (read / write request) en houdt 
		//	dit bij in de array van structs _socket_fds (door .events aan te passen)
		poll_count = poll (&_socket_fds[0], _number_of_socket_fds, -1);
		if (poll_count == -1) {
			exitWithError("Poll count = negative (TcpServer::startListen()");
		}
		lookupActiveSocket();			// vervolgens kunnen we kijken op welke socket er iets is gebeurd
	}	
	// 	receiveRequest();
	// 	sendResponse();
	
}

void TcpServer::acceptConnection() 
{
	struct pollfd	add_to_socket_fd;

	add_to_socket_fd.fd = accept(_socket_fds[0].fd, (sockaddr *)&_socketAddress, &_socketAddress_len);
	if (add_to_socket_fd.fd == -1)
		exitWithError("ERROR: accept() (TcpServer::acceptConnection");

	fcntl(add_to_socket_fd.fd, F_SETFL, O_NONBLOCK);			// nodig om elke socket op non blocking te zetten?


	//// set events for added socket?
	/*
	add_to_socket.events = POLLIN | POLLOUT

	hoe weten we of een nieuwe connectie POLLIN, POLLOUT of POLLIN | POLLOUT moet zijn?

	*/

	add_to_socket_fd.events = POLLIN;	
	_socket_fds.push_back(add_to_socket_fd);
	_number_of_socket_fds++;
	std::cout << "Server accepted incoming connection from ADDRESS: "
			<< inet_ntoa(_socketAddress.sin_addr) << "; PORT: " 
			<< ntohs(_socketAddress.sin_port) << std::endl;
}

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

	_request.initHTTPRequest(std::string(buff));
	if (!_request.isValidMethod())
		exitWithError("Invalid method, only GET, POST and DELETED are supported");
	std::ostringstream ss;
	ss	<< "Received request: Method = " << _request.getMethod()
		<< " URI = " 					 << _request.getURI()
		<< " HTTP Version = " 			 << _request.getHTTPVersion();
	log(ss.str());
}

void TcpServer::sendResponse()
{
	// class BuildResponse respons(_request, _config);
	// _serverMessage = respons.getMessage("200 OK");
	// long bytesSent = write(_new_socket, _serverMessage.c_str(), _serverMessage.size());
	// if (bytesSent == (long)_serverMessage.size())
	// 	log(_serverMessage.substr(0, _serverMessage.find('\n')));
	// 	// log("\n");
	// else
	// 	log("Error sending response to client");
}

// Finds the socket where poll() found activity 
int	TcpServer::lookupActiveSocket()
{
	for (int i = 0; i < _number_of_socket_fds; i++) {
		if (_socket_fds[i].revents & POLLIN) {					// revents houdt bij of er iets gebeurd is (POLLIN == receive / read, POLLOUT == send / write)
			if (_socket_fds[i].fd == _socket_fds[0].fd) 		// als dit listeneing socket is betekent dat dat er een nieuwe connectie is 
				acceptConnection();
			else 
				receiveRequest(i);
		} else if (_socket_fds[i].revents & POLLOUT) {
			size_t	bytes_send;

			bytes_send = send(_socket_fds[i].fd, &_serverMessage, _serverMessage.size(), 0);
			if (bytes_send < 0)
				std::cout << "Send error in TcpServer::lookupActiveSocket()" << std::endl;
			if (bytes_send < _serverMessage.size())
				std::cout << "Not all bytes send!" << std::endl;

		}

	}  													 		
}



} // namespace http
