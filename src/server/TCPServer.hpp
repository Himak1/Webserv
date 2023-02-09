#ifndef TCP_SERVER_HPP
# define TCP_SERVER_HPP

# include "../request/Request.hpp"
# include "../server/Configuration.hpp"

# include <stdio.h>
# include <sys/socket.h>
# include <arpa/inet.h>
# include <stdlib.h>
# include <string>

#include <poll.h>

namespace http
{;

typedef struct s_socket {
	unsigned int		socket_address_len;
	struct sockaddr_in	socket_info;
	std::string			server_message;
}	t_socket;

class TcpServer
{
public:
	TcpServer(class Configuration configuration);
	~TcpServer();
	void				startListen();

private:
	class Configuration			_config;
	class Request				_request;
	
	std::vector<struct pollfd>	_pollFds;			
	std::vector<t_socket>		_socketInfo;
	unsigned int				_nbListeningSockets;
	bool						_isServerRunning;

	void						newConnection(int);
	void						closeConnection(int);
	void						closeServer();
	void						receiveRequest(int);
	void						sendResponse(int);
	int 						startServer();
	void						setUpListeningSockets();
	void						lookupActiveSocket();
};

}			// namespace http

# endif		// TCP_SERVER_HPP
