# include "../defines.hpp"
# include "TCPServer.hpp"
# include "../response/Response.hpp"
# include "../utils/memory.hpp"
# include "../utils/strings.hpp"
# include "../utils/log.hpp"

# include <sys/socket.h>
# include <sys/types.h>
# include <netdb.h>
# include <poll.h>
# include <fcntl.h>
# include <iostream>
# include <sstream>
# include <unistd.h>
# include <fstream>

using namespace std;

void logStartupMessage(struct sockaddr_in _socketAddress)
{
	ostringstream ss;

	ss	<< "Listening on PORT: "
		<< ntohs(_socketAddress.sin_port);
	log_receive(ss.str());
}

namespace http
{

			// CONSTRUCTORS

TCPServer::TCPServer(vector<Configuration*> configList) :
		_configList(configList),
		_nbListeningSockets(0)
{
	try {
		setListeningSockets();
	} catch (exception& e) {
		cout << e.what() << endl;
		exit(EXIT_FAILURE);
	}	
	_isServerRunning = true;
	startPolling();
}

			// DESTRUCTORS

TCPServer::~TCPServer()
{
	cout << "Closed server" << endl;
}
			
			// SETUP SERVER
void	TCPServer::setListeningSockets()
{
	struct pollfd	poll_fd;
	t_socket		listener;
	int				re_use = 1, i = 0;

	for (vector<Configuration*>::iterator it = _configList.begin(); it != _configList.end(); it++, i++) {
		ft_memset(&listener, 0, sizeof(listener));

		poll_fd.fd = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
		if (poll_fd.fd == -1)
			throw SockCreateFail();	
		if (setsockopt(poll_fd.fd, SOL_SOCKET, SO_REUSEADDR, &re_use, sizeof(re_use)) == -1)
			throw SockOptionsFail();

		setSocketStruct(&listener, (*it)->getPort(), (*it)->getHost());

		if (bind(poll_fd.fd, (struct sockaddr*)&listener.socket_address_info, sizeof(listener.socket_address_info)) == -1)
			throw SockBindingFail();
		if (listen(poll_fd.fd, SOMAXCONN) == -1)
			throw ListenFail();
		if (fcntl(poll_fd.fd, F_SETFL, O_NONBLOCK) == -1)
			throw SockNoBlock();

		listener.socket_address_len = sizeof(listener.socket_address_info);

		poll_fd.events = POLLIN;
		_socketInfo.push_back(listener);
		_pollFds.push_back(poll_fd);
		_nbListeningSockets++;

		logStartupMessage(_socketInfo[i].socket_address_info);
	}
}

void	TCPServer::setSocketStruct(t_socket *listener, int port, string host)
{
	struct addrinfo 	setup, *result;
	int					status;
	ft_memset(&setup, 0, sizeof setup);
	setup.ai_family = AF_INET;
	setup.ai_socktype = SOCK_STREAM;

	string po = convertToString(port);
	if ((status = getaddrinfo(&host[0], &po[0], &setup, &result)) != 0) {
        cerr << gai_strerror(status) << endl;;
        exit(1);
    }
	struct sockaddr_in *ipv4 = (struct sockaddr_in *)result->ai_addr;
	listener->socket_address_info = *ipv4;
	freeaddrinfo(result);
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
		if (poll_count == -1)
			exitWithError("poll failed. Exiting");
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
			} catch (exception& e) {
				cout << e.what() << endl;
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
void 	TCPServer::receiveRequest(int idx)
{
	char buff[BUFFER_SIZE] = {0};

	int bytes_received = read(_pollFds[idx].fd, buff, BUFFER_SIZE - 1);	
	if (bytes_received <= 0) {
		if (bytes_received < 0) 
			closeConnection(idx);
		else
			_pollFds[idx].events = POLLOUT;
		return ;
	}
	buff[bytes_received] = 0;
	_socketInfo[idx].client_request += buff;
	
	_pollFds[idx].events = POLLIN;
	_pollFds[idx].events += POLLOUT;
}

void 	TCPServer::sendResponse(int idx)
{
	int			bytes_send;

	if (serverMsgIsEmpty(idx)) {
		_request.initRequest(_socketInfo[idx].client_request);
		log_receive(_request.getMethod() + " " + _request.getURI() + " " + _request.getHTTPVersion());

		class 		Response respons(_request, *_configList[_socketInfo[idx].config_idx]);
		
		_socketInfo[idx].server_message = respons.getMessage();
		log_response(_socketInfo[idx].server_message);
	}
	
	bytes_send = write(_pollFds[idx].fd, _socketInfo[idx].server_message.c_str(), _socketInfo[idx].server_message.size());
	if (bytes_send < 0)
		cerr << "Send error on socket " << idx << endl;
	if (bytes_send <= 0)
		closeConnection(idx);

	_socketInfo[idx].server_message.erase(0, bytes_send);
	if (serverMsgIsEmpty(idx))
		closeConnection(idx);
	else
		_pollFds[idx].events = POLLOUT;
}

void TCPServer::newConnection(int idx)
{
	struct pollfd	new_pollfd;
	t_socket		new_socket;
	socklen_t		socket_len;

	new_socket.socket_address_len = sizeof(new_socket.socket_address_info);
	new_pollfd.fd = accept(_pollFds[idx].fd, (sockaddr *)&new_socket.socket_address_info, &socket_len);	
	if (new_pollfd.fd == -1)
		throw AcceptFail();

	new_socket.socket_address_len = socket_len;

	setFileDescrOptions(new_pollfd.fd);				
	new_pollfd.events = POLLIN;
	new_socket.config_idx = idx;
	_pollFds.push_back(new_pollfd);
	_socketInfo.push_back(new_socket);
}

void	TCPServer::closeConnection(int idx)
{
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
