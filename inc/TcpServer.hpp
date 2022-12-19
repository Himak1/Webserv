#ifndef TCP_SERVER_HPP
# define TCP_SERVER_HPP

# include <HTTPRequest.hpp>

# include <stdio.h>
# include <sys/socket.h>
# include <arpa/inet.h>
# include <stdlib.h>
# include <string>

namespace http
{

	class TcpServer
	{
	public:
		TcpServer(std::string ip_address, int port);
		~TcpServer();
		void                startListen();

	private:
		std::string         _ip_address;
		int                 _port;
		int                 _socket;
		int                 _new_socket;
		
		// long                _incomingMessage;
		struct sockaddr_in  _socketAddress;
		unsigned int        _socketAddress_len;
		std::string         _serverMessage;
		class HTTPRequest	_request;
		int                 startServer();
		void                closeServer();
		void                acceptConnection(int &new_socket);
		void				receiveRequest();
		std::string         buildResponse();
		void                sendResponse();
	};

} // namespace http

#endif
