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
	// const unsigned int	socket_idx;
	// int					socket_address;		// is ook een struct?
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


	// houden
	std::vector<struct pollfd>	_pollFds;			// alle structs moeten hierin voor poll
	std::vector<Socket>			_sockets;   

	unsigned int				_nbListeningSockets;
	bool						_isServerRunning;

	class Configuration			_config;
	class Request				_request;
	/****************************************************************/
	/* 	_pollFds loopt gelijk met _socketInfo, dus de socket info	*/
	/*	van _pollFds[i] staat in _socketInfo[i]						*/
	/*	deze kunnen niet in 1 class of struct komen omdat _pollFds	*/
	/*	een array moet zijn (poll() heeft deze nodig)				*/
	/****************************************************************/
	
	
	std::vector<t_socket>		_socketInfo;   /// = Socket



	std::string					_serverMessage;
	std::string					_unsendServerMessage;












	void						newConnection(int);
	void						closeClientConnection(int);
	void						closeServer();
	void						receiveRequest(int);
	void						sendResponse(int);
	int 						startServer();
	void						setUpListeningSockets();
	void						lookupActiveSocket();

	// std::vector<t_socket>		_listeningSockets;
	// std::vector<t_socket>		_clientSockets;
	// unsigned int				_nbListeningSockets;
	// std::vector<Client *>		_clients;

	// std::vector<int>		_socketFdsServer;			
	// unsigned int			_nbOfServerSocketFds;
	// int					_new_socket;
	// std::vector<struct sockaddr_in>	_socketAddressServer;		
	// std::vector<unsigned int>		_socketAddressLenServer;
};

} // namespace http

#endif

