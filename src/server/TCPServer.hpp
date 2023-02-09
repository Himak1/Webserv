#ifndef TCP_SERVER_HPP
# define TCP_SERVER_HPP

# include "../request/Request.hpp"
# include "../server/Configuration.hpp"
# include "Socket.hpp"

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
	
	std::vector<struct pollfd>	_pollFds;
	std::vector<Socket>			_sockets;   

	unsigned int				_nbListeningSockets;

	bool						_isServerRunning;

	class Configuration			_config;
	class Request				_request;
	

	void						newClientConnection(int);
	void						closeClientConnection(int);
	void						closeServer();
	void						receiveRequest(int);
	void 						createServerMessage(int);
	void						sendResponse(int);
	int 						startServer();
	void						setUpListeningSockets();
	void						lookupActiveSocket();


};

} // namespace http

#endif

