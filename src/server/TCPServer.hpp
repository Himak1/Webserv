#ifndef TCP_SERVER_HPP
# define TCP_SERVER_HPP

# include "../request/Request.hpp"
# include "../configure/Configuration.hpp"

# include <exception>
# include <stdio.h>
# include <sys/socket.h>
# include <arpa/inet.h>
# include <stdlib.h>
# include <string>
# include <vector>
# include <poll.h>

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
	TCPServer(std::vector<Configuration*>);
	~TCPServer();

	void				startPolling();
	
private:
		// Private members 
	std::vector<Configuration*>	_configList;
	std::vector<struct pollfd>	_pollFds;			
	std::vector<t_socket>		_socketInfo;
	class Request				_request;

	unsigned int				_nbListeningSockets;
	bool						_isServerRunning;

		// Private member functions
 	void						newConnection(int);
	void						closeConnection(int);
	void						closeServer();
	void						receiveRequest(int);
	void 						createServerMessage(int);
	bool						serverMsgIsEmpty(int);
	void						sendResponse(int);
	int 						startServer();
	void						setupListeningSockets();
	void						setupSocketStruct(t_socket *);
	void						setFileDescrOptions(int);
	void						lookupActiveSocket();

		// Private exceptions
	struct SockBindingFail : public exception {
		const char * what () const throw () {
    		return "Failed to bind socket. Exiting program";
		}
	};	
	struct ListenFail : public exception {
		const char * what () const throw () {
    		return "Failure to listen to socket. Exiting program";
		}
	};	
	struct SockNoBlock : public exception {
		const char * what () const throw () {
    		return "Failure to set socket to non-blocking. Exiting program";
		}
	};	
	struct SockCreateFail : public exception {
		const char * what () const throw () {
    		return "Failure to create socket. Exiting program";
		}
	};	




};

}			// namespace http

# endif		// TCP_SERVER_HPP
