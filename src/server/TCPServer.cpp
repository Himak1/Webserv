#include "TCPServer.hpp"
#include "../response/Response.hpp"
#include "../utils/log.hpp"

#include <iostream>
#include <sstream>
#include <unistd.h>
#include <fstream>

namespace
{
	const int BUFFER_SIZE = 30720;

	void logStartupMessage(struct sockaddr_in _socketAddress)
	{
		std::ostringstream ss;
		ss	<< "Listening on ADDRESS: "
			<< inet_ntoa(_socketAddress.sin_addr) 
			<< "  PORT: "
			<< ntohs(_socketAddress.sin_port) 
			<< "\n";
		log(ss.str());
	}

	void exitWithError(const std::string &errorMessage)
	{
		log("ERROR: " + errorMessage);
		exit(1);
	}
}

namespace http
{
	// CONSTRUCTOR
	TCPServer::TCPServer(class Configuration configuration)
		: _config(configuration), _socket(), _new_socket(),
		_socketAddress(), _socketAddress_len(sizeof(_socketAddress))
	{
		_socketAddress.sin_family = AF_INET;
		_socketAddress.sin_port = htons(_config.getPort());
		_socketAddress.sin_addr.s_addr = inet_addr(_config.getIP().c_str());

		if (startServer() != 0) {
			std::ostringstream ss;
			ss << "Failed to start server with PORT: " << ntohs(_socketAddress.sin_port);
			log(ss.str());
		}
	}

	// DESTRUCTOR
	TCPServer::~TCPServer()
	{
		closeServer();
	}

	// PRIVATE FUNCTIONS
	int TCPServer::startServer()
	{
		_socket = socket(AF_INET, SOCK_STREAM, 0);
		if (_socket < 0) {
			exitWithError("Cannot create socket");
			return 1;
		}

		if (bind(_socket, (sockaddr *)&_socketAddress, _socketAddress_len) < 0) {
			exitWithError("Cannot connect socket to address");
			return 1;
		}

		return 0;
	}

	void TCPServer::closeServer()
	{
		close(_socket);
		close(_new_socket);
		exit(0);
	}

	void TCPServer::startListen()
	{
		if (listen(_socket, 20) < 0)
			exitWithError("Socket listen failed");

		logStartupMessage(_socketAddress);

		while (true) {
			acceptConnection(_new_socket);
			receiveRequest();
			sendResponse();
			close(_new_socket);
		}
	}

	void TCPServer::acceptConnection(int &new_socket) 
	{
		new_socket = accept(_socket, (sockaddr *)&_socketAddress, &_socketAddress_len);
		if (new_socket < 0) {
			std::ostringstream ss;
			ss	<< "Server failed to accept incoming connection from ADDRESS: "
				<< inet_ntoa(_socketAddress.sin_addr) << "; PORT: " 
				<< ntohs(_socketAddress.sin_port);
			exitWithError(ss.str());
		}
	}

	void TCPServer::receiveRequest()
	{
		char 	buffer[BUFFER_SIZE] = {0};

		int bytesReceived = read(_new_socket, buffer, BUFFER_SIZE);
		if (bytesReceived < 0)
			exitWithError("Failed to read bytes from client socket connection");

		_request.initRequest(std::string(buffer));

		if (!_request.isValidMethod())
			exitWithError("Invalid HTTP version (only HTTP/1.1 is supported) " \
				"or invalid method (only GET, POST and DELETED are supported)");

		// log(buffer);

		std::ostringstream ss;
		ss	<< _request.getMethod() << " "
			<< _request.getURI();
		log(ss.str());
	}

	void TCPServer::sendResponse()
	{
		class Response respons(_request, _config);
		_serverMessage = respons.getMessage();

		long bytesSent = write(_new_socket, _serverMessage.c_str(), _serverMessage.size());

		if (bytesSent == (long)_serverMessage.size())
			log(_serverMessage.substr(0, _serverMessage.find('\n')));
		else
			log("Error sending response to client");
	}

} // namespace http
