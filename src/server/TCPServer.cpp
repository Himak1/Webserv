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

			//	SERVER EVENT HANDLING
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

void 	TCPServer::newConnection(int idx)
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

} // namespace 
