# include "Socket.hpp"

# include <vector>
# include <poll.h>

Socket::Socket() //: _socket_address_len(sizeof(socket_info)) {
	{
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

socklen_t			Socket::getSocketAddressLen() const
{
	return _socket_address_len;
}

void				Socket::setSocketAddressLen(unsigned int length)
{
	_socket_address_len = length;
}
