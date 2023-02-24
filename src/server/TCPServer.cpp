# include "../defines.hpp"
# include "TCPServer.hpp"
# include "../response/Response.hpp"
# include "../utils/memory.hpp"
# include "../utils/strings.hpp"
# include "../utils/log.hpp"

# include <cstdlib>
# include <cstring>
# include <sys/socket.h>
# include <sys/types.h>
# include <netdb.h>
# include <errno.h>
# include <poll.h>
# include <fcntl.h>
# include <iostream>
# include <sstream>
# include <unistd.h>
# include <fstream>
# include <string.h>

using namespace std;

void logStartupMessage(struct sockaddr_in _socketAddress)
{
	std::ostringstream ss;

	ss	<< "Listening on ADDRESS: "
		<< inet_ntoa(_socketAddress.sin_addr) 
		<< " PORT: "
		<< ntohs(_socketAddress.sin_port);
	log_receive(ss.str());
}

namespace http
{

			// CONSTRUCTORS

TCPServer::TCPServer(std::vector<Configuration*> configList) :
		_configList(configList),
		_nbListeningSockets(0)
{
	try {
		setListeningSockets();
	} catch (std::exception& e) {
		if (DEBUG_INFO)
			std::cout << std::strerror(errno) << std::endl;
		std::cout << e.what() << std::endl;
		std::exit(EXIT_FAILURE);
	}	
	_isServerRunning = true;	// tmp?
	startPolling();
}

			// DESTRUCTORS

TCPServer::~TCPServer()
{
	std::cout << "Closed server" << std::endl;
}
			
			// SETUP SERVER
void	TCPServer::setListeningSockets()
{
	struct pollfd	poll_fd;
	t_socket		listener;
	int				re_use = 1, i = 0;

	for (std::vector<Configuration*>::iterator it = _configList.begin(); it != _configList.end(); it++, i++) {
		ft_memset(&listener, 0, sizeof(listener));

		poll_fd.fd = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
		if (poll_fd.fd == -1)
			throw SockCreateFail();	
		if (setsockopt(poll_fd.fd, SOL_SOCKET, SO_REUSEADDR, &re_use, sizeof(re_use)) == -1)
			throw SockOptionsFail();

		setSocketStruct(&listener, (*it)->getPort());

		if (bind(poll_fd.fd, (struct sockaddr*)&listener.socket_address_info, sizeof(listener.socket_address_info)) == -1)
			throw SockBindingFail();
		if (listen(poll_fd.fd, SOMAXCONN) == -1)
			throw ListenFail();
		if (fcntl(poll_fd.fd, F_SETFL, O_NONBLOCK) == -1)
			throw SockNoBlock();

		listener.socket_address_len = sizeof(listener.socket_address_info);		// nodig?

		poll_fd.events = POLLIN;
		_socketInfo.push_back(listener);
		_pollFds.push_back(poll_fd);
		_nbListeningSockets++;

		logStartupMessage(_socketInfo[i].socket_address_info);
	}
}

void	TCPServer::setSocketStruct(t_socket *listener, int port)
{
	listener->socket_address_info.sin_addr.s_addr = INADDR_ANY;
	listener->socket_address_info.sin_family = AF_INET;
	listener->socket_address_info.sin_port = htons(port);
}

void	TCPServer::setFileDescrOptions(int file_descr)
{
	int	re_use = 1;

	if (fcntl(file_descr, F_SETFL, O_NONBLOCK) == -1)
		throw SockNoBlock();
	if (setsockopt(file_descr, SOL_SOCKET, SO_REUSEADDR, &re_use, sizeof(re_use)) == -1) 
		throw SockOptionsFail();			
	if (setsockopt(file_descr, SOL_SOCKET, SO_REUSEPORT, &re_use, sizeof(re_use)) == -1) 
		throw SockOptionsFail();
}

			// SERVER LOOP
void	TCPServer::startPolling()
{
	int	poll_count;

	while (_isServerRunning) {
		poll_count = poll(&_pollFds[0], _pollFds.size(), POLL_TIMEOUT);		
		if (poll_count == -1) {
			if (DEBUG_INFO)
				std::cout << strerror(errno) << std::endl;
			exitWithError("poll failed. Exiting");
		}
		lookupActiveSocket();
	}	
}

void	TCPServer::lookupActiveSocket()
{
	unsigned int i;

	for (i = 0; i < _nbListeningSockets; i++) {
		if (_pollFds[i].revents == 0)
			continue ;
		else {
			try {
				newConnection(i);	
			} catch (std::exception& e) {
				if (DEBUG_INFO)
					std::cout << std::strerror(errno) << std::endl;
				std::cout << e.what() << std::endl;
			}	
		}
	}

	for (unsigned int j = 0; j < (_pollFds.size() - _nbListeningSockets); j++, i++) {
		if 	    (_pollFds[i].revents == 0)			continue;
		else if (_pollFds[i].revents & POLLIN) 		receiveRequest(i); 	
		else if (_pollFds[i].revents & POLLOUT) 	sendResponse(i);	
		else if (_pollFds[i].revents & POLLHUP) 	closeConnection(i);	
		else if (_pollFds[i].revents & POLLNVAL) 	closeConnection(i);
	}	
}

			//	SERVER EVENT HANDLING
void TCPServer::receiveRequest(int idx)
{
	unsigned int buffer_size = _configList[_socketInfo[idx].config_idx]->getClientMaxBodySize();
	char * buff = (char *)calloc(buffer_size + 1, sizeof(char));
	if (!buff) {
		if (DEBUG_INFO)
			std::cout << "Calloc failure: " << std::strerror(errno) << std::endl;
		return ;
	}

	int bytes_received = read(_pollFds[idx].fd, buff, buffer_size);	
	if (bytes_received <= 0) {
		if (bytes_received < 0)
			std::cout << std::strerror(errno) << std::endl;
		else if (DEBUG_INFO)	
			std::cout << "Socket fd " << _pollFds[idx].fd << " closed their connection." << std::endl;
		closeConnection(idx);
		free (buff);
		return ;
	}
	string buffer = buff;
	free (buff);
	_request.initRequest(buffer);										
	class Response respons(_request, *_configList[_socketInfo[idx].config_idx]);					// tmp?

	_pollFds[idx].events = POLLOUT;
	log_receive(_request.getMethod() + " " + _request.getURI() + " " + _request.getHTTPVersion());
}

void TCPServer::sendResponse(int idx)
{
	int			bytes_send;
	class 		Response respons(_request, *_configList[_socketInfo[idx].config_idx]);

	if (serverMsgIsEmpty(idx)) {
		_socketInfo[idx].server_message = respons.getMessage();
		log_response(_socketInfo[idx].server_message);
	}
	
	bytes_send = write(_pollFds[idx].fd, _socketInfo[idx].server_message.c_str(), _socketInfo[idx].server_message.size());
	if (bytes_send <= 0) {
		if (bytes_send < 0) {
			std::cout << "Send error in TCPServer::sendResponse()" << std::endl;
			closeConnection(idx);
		}
		else {
			std::cout << "Zero bytes send. Need handler?" << std::endl;							// tmp
		}
	}
	_socketInfo[idx].server_message.erase(0, bytes_send);
	if (serverMsgIsEmpty(idx)) {
		_pollFds[idx].events = POLLIN;
		closeConnection(idx); 				// php error?
	}
	else {
		_pollFds[idx].events = POLLOUT;
	}	
}

void TCPServer::newConnection(int idx)
{
	struct pollfd	new_pollfd;
	t_socket		new_socket;
	socklen_t		socket_len;

	new_socket.socket_address_len = sizeof(new_socket.socket_address_info);
	new_pollfd.fd = accept(_pollFds[idx].fd, (sockaddr *)&new_socket.socket_address_info, &socket_len);	
	if (new_pollfd.fd == -1) {
		throw AcceptFail();
	}
	new_socket.socket_address_len = socket_len;

	setFileDescrOptions(new_pollfd.fd);				
	new_pollfd.events = POLLIN;
	new_socket.config_idx = idx;
	_pollFds.push_back(new_pollfd);
	_socketInfo.push_back(new_socket);

	if (DEBUG_INFO)
		std::cout << "Server accepted incoming connection from ADDRESS: "
			<< inet_ntoa(new_socket.socket_address_info.sin_addr) << "; PORT: " 
			<< ntohs(new_socket.socket_address_info.sin_port) << "; Socket fd: " << _pollFds.back().fd << std::endl;
}

void	TCPServer::closeConnection(int idx)
{
	if (DEBUG_INFO)
		std::cout << "closeConnection called for idx " << idx << ", closing socket fd " << _pollFds[idx].fd 
				<< " PORT: " << ntohs(_socketInfo[idx].socket_address_info.sin_port) << std::endl;
	close(_pollFds[idx].fd);
	_pollFds.erase(_pollFds.begin() + idx);						
	_socketInfo.erase(_socketInfo.begin() + idx);
}

bool	TCPServer::serverMsgIsEmpty(int idx)
{
	if (_socketInfo[idx].server_message.empty())
		return true;
	else
		return false;	
}

} // namespace http
