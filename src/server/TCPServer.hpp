#ifndef TCP_SERVER_HPP
# define TCP_SERVER_HPP

# include "../request/Request.hpp"
# include "../configure/Configuration.hpp"

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
	unsigned int		config_idx;
}	t_socket;

class TCPServer
{
public:
	TCPServer(std::list<Configuration*>);
	~TCPServer();
	void				startListen();

private:
	// class Configuration			_config;
	std::list<Configuration*>	_configList;
	class Request				_request;
	
	std::vector<struct pollfd>	_pollFds;			
	std::vector<t_socket>		_socketInfo;
	unsigned int				_nbListeningSockets;

	bool						_isServerRunning;

	void						socketDefault(t_socket *);
	void						newConnection(int);
	void						closeConnection(int);
	void						closeServer();
	void						receiveRequest(int);
	void 						createServerMessage(int);
	void						sendResponse(int);
	int 						startServer();
	void						setUpListeningSockets();
	void						lookupActiveSocket();
};

}			// namespace http

# endif		// TCP_SERVER_HPP
