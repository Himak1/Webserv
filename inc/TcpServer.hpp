#ifndef TCP_SERVER_HPP
# define TCP_SERVER_HPP

# include <HTTPRequest.hpp>
# include <Configuration.hpp>

# include <stdio.h>
# include <sys/socket.h>
# include <arpa/inet.h>
# include <stdlib.h>
# include <string>


#include <poll.h>

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
		int					_listening_socket;
		// int					_new_socket;
		struct sockaddr_in	_socketAddress;
		unsigned int		_socketAddress_len;
		std::string			_serverMessage;
		int 				startServer();
		void				closeServer();
		void				acceptConnection();
		void				receiveRequest();
		void				sendResponse();


		// TOEGEVOEGD DOOR JONATHAN

		std::vector<pollfd>	_socket_fds;
    	nfds_t				_number_of_socket_fds;


	};

} // namespace http

#endif
