#ifndef TCP_SERVER_HPP
# define TCP_SERVER_HPP

# include <HTTPRequest.hpp>
# include <Configuration.hpp>

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
		TcpServer(class Configuration configuration);
		~TcpServer();
		void				startListen();

	private:
		class Configuration	_config;
		class HTTPRequest	_request;
		int					_socket;
		int					_new_socket;
		struct sockaddr_in	_socketAddress;
		unsigned int		_socketAddress_len;
		std::string			_serverMessage;
		int 				startServer();
		void				closeServer();
		void				acceptConnection(int &new_socket);
		void				receiveRequest();
		void				sendResponse();
	};

} // namespace http

#endif
