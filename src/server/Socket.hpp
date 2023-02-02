#ifndef SOCKET_HPP
# define SOCKET_HPP

#include <vector>
#include <poll.h>
#include <string>

class Socket 
{
public :
    Socket();
	~Socket();

	std::string			getServerMessage() const;
	void 				setServerMessage(std::string);
	unsigned int		getSocketAddressLen() const;

	struct sockaddr_in	socket_info;

	// void				setSocketAddressLen(unsigned int);
private :

	// std::vector<pollfd>	_pollFd;
   	// static nfds_t		_number_of_socketFds;



	unsigned int		_socket_address_len;
	// std::string			_server_message;

	std::string			_serverMessage;
	std::string			_unsendServerMessage;





}




#endif          // SOCKET_HPP