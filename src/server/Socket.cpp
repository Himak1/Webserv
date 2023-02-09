# include "Socket.hpp"

# include <vector>
# include <poll.h>
# include <iostream>


Socket::Socket() : _socket_address_len(sizeof(sockaddr_in)) 
{
	// std::cout << "created socket" << std::endl;

	// _socket_address_len = (socklen_t) sizeof(sockaddr_in);
	// std::cout << _socket_address_len << std::endl;
	
	
	// _number_of_socketFds = 0;
 	// _pollFd.push_back(*this);
	// _number_of_socketsFds++;
}

Socket::~Socket() {
}

std::string			Socket::getServerMessage() const
{
	return _serverMessage;
}

void 				Socket::setServerMessage(std::string message)
{
	_serverMessage = message;
}

void				Socket::setSocketAddressLen(unsigned int set)
{
	_socket_address_len = set;
}

unsigned int		Socket::getSocketAddressLen() const
{
	return _socket_address_len;
}
