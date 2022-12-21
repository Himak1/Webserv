#include "TcpServer.hpp"
#include "BuildResponse.hpp"

#include <iostream>
#include <sstream>
#include <unistd.h>
#include <fstream>

namespace
{
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
{
	// CONSTRUCTOR
	TcpServer::TcpServer(class Configuration configuration)
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
	TcpServer::~TcpServer()
	{
		closeServer();
	}

	// PRIVATE FUNCTIONS
	int TcpServer::startServer()
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

	void TcpServer::closeServer()
	{
		close(_socket);
		close(_new_socket);
		exit(0);
	}

	void TcpServer::startListen()
	{
		if (listen(_socket, 20) < 0)
			exitWithError("Socket listen failed");

		logStartupMessage(_socketAddress);

		while (true) {
			log("\n====== Waiting for a new connection ======\n");

			acceptConnection(_new_socket);
			receiveRequest();
			sendResponse();
			close(_new_socket);
		}
	}

	void TcpServer::acceptConnection(int &new_socket) 
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

	void TcpServer::receiveRequest()
	{
		char 	buffer[BUFFER_SIZE] = {0};

		int bytesReceived = read(_new_socket, buffer, BUFFER_SIZE);
		if (bytesReceived < 0)
			exitWithError("Failed to read bytes from client socket connection");

		_request.initHTTPRequest(std::string(buffer));

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
		class BuildResponse respons(_request, _config);
		_serverMessage = respons.getMessage("200 OK");

		long bytesSent = write(_new_socket, _serverMessage.c_str(), _serverMessage.size());

		if (bytesSent == (long)_serverMessage.size())
			log(_serverMessage.substr(0, _serverMessage.find('\n')));
			// log("\n");
		else
			log("Error sending response to client");
	}

} // namespace http
