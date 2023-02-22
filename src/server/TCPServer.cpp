# include "TCPServer.hpp"
# include "../response/Response.hpp"
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



# define POLL_TIMEOUT 200	 // in ms
# define DEBUG_INFO 1


/* #################################################################################################### */
/* #################################################################################################### */	//		TMP		
/* #################################################################################################### */	//		TMP
/* ###########																				###########	*/	//		TMP
/* ###########		WERKT WEL (leidt example.com naar onze localhost)						###########	*/	//		TMP
/* ###########		curl --resolve example.com:80:127.0.0.1 http://example.com/index.html	###########	*/	//		TMP
/* ###########																				###########	*/	//		TMP
/* ###########		WERKT NIET (bezoekt example.com)										###########	*/	//		TMP
/* ###########		curl --resolve example.com:8000:127.0.0.1 http://example.com/index.html	########	*/	//		TMP
/* ###########																				###########	*/	//		TMP
/* ###########																				###########	*/	//		TMP
/* ###########		Voordat we runnen moet het max aantal open file descriptors van 		###########	*/	//		TMP
/* ###########		het OS gecheckt worden (wij hebben per connectie een file descriptor 	###########	*/	//		TMP
/* ###########		nodig).																 	###########	*/	//		TMP
/* ###########		Dit doe je door in terminal 'ulimit -n' in te typen.					###########	*/	//		TMP
/* ###########		Om de limiet te verhogen type je: 'ulimit -n X'	met X voor aantal fds	###########	*/	//		TMP
/* ###########		Voor 1000 connecties heb je ca 1020+ fds nodig							###########	*/	//		TMP
/* ###########																				###########	*/	//		TMP
/* ###########		In siege.conf (root/.siege/)											###########	*/	//		TMP
/* ###########		connection = keep-alive													###########	*/	//		TMP
/* ###########		Siege set een port die gesloten wordt op TIMEOUT, die poort kan dan 	###########	*/	//		TMP
/* ###########		een tijdje niet meer gebruikt worden. Door 'connection = keep-alive'	###########	*/	//		TMP
/* ###########		kan dit wel. omdat wij poorten hergebruiken moet dit dus				###########	*/	//		TMP
/* ###########																				###########	*/	//		TMP
/* #################################################################################################### */	//		TMP
/* #################################################################################################### */
/* #################################################################################################### */	


#define QUEU_LIMIT_LISTEN 20

using namespace std;

const int BUFFER_SIZE = 30720;

void logStartupMessage(struct sockaddr_in _socketAddress)
{
	std::ostringstream ss;

	ss	<< "\n*** Listening on ADDRESS: "
		<< inet_ntoa(_socketAddress.sin_addr) 
		<< " PORT: "
		<< ntohs(_socketAddress.sin_port) 
		<< " ***\n\n";
	log_receive(ss.str());
}

namespace http
{;

			// CONSTRUCTORS

TCPServer::TCPServer(std::vector<Configuration*> configList) :
		_configList(configList),
		_nbListeningSockets(0)
{
	try {
		setupListeningSockets();
	} catch (std::exception& e) {
		if (DEBUG_INFO)
			cout << std::strerror(errno) << endl;
		cout << e.what() << endl;
		std::exit(1);
	}	

	_isServerRunning = true;	// tmp?
	startPolling();
}

			// DESTRUCTORS

TCPServer::~TCPServer()
{
	closeServer();
}

			// PRIVATE FUNCTIONS

void	TCPServer::setupListeningSockets()
{
	struct pollfd	poll_fd;
	t_socket		listener;
	int				re_use = 1, i = 0;

	for (std::vector<Configuration*>::iterator it = _configList.begin(); it != _configList.end(); it++, i++) {
		std::memset(&listener, 0, sizeof(listener));

		poll_fd.fd = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
		if (poll_fd.fd == -1)
			throw SockCreateFail();	
		if (setsockopt(poll_fd.fd, SOL_SOCKET, SO_REUSEADDR, &re_use, sizeof(re_use)) == -1)
			throw SockOptionsFail();

		setupSocketStruct(&listener, (*it)->getPort());

		if (bind(poll_fd.fd, (struct sockaddr*)&listener.socket_info, sizeof(listener.socket_info)) == -1)
			throw SockBindingFail();
		if (listen(poll_fd.fd, SOMAXCONN) == -1)
			throw ListenFail();
		if (fcntl(poll_fd.fd, F_SETFL, O_NONBLOCK) == -1)
			throw SockNoBlock();

		listener.socket_address_len = sizeof(listener.socket_info);		// nodig?

		poll_fd.events = POLLIN;
		_socketInfo.push_back(listener);
		_pollFds.push_back(poll_fd);
		_nbListeningSockets++;

		logStartupMessage(_socketInfo[i].socket_info);
	}
}

void	TCPServer::setupSocketStruct(t_socket *listener, int port)		// tmp?
{
	listener->socket_info.sin_addr.s_addr = INADDR_ANY;
	listener->socket_info.sin_family = AF_INET;
	listener->socket_info.sin_port = htons(port);
}

void TCPServer::startPolling()
{
	int	poll_count;

	while (_isServerRunning) {
		poll_count = poll (&_pollFds[0], _pollFds.size(), POLL_TIMEOUT);		
		if (poll_count == -1) {
			if (DEBUG_INFO)
				cout << strerror(errno) << endl;
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
		else
			newConnection(i);	
	}

	for (unsigned int j = 0; j < (_pollFds.size() - _nbListeningSockets); j++, i++) {
		if 	    (_pollFds[i].revents == 0)			continue;
		else if (_pollFds[i].revents & POLLIN) 		receiveRequest(i); 	
		else if (_pollFds[i].revents & POLLOUT) 	sendResponse(i);	
		else if (_pollFds[i].revents & POLLHUP) 	closeConnection(i);	
		else if (_pollFds[i].revents & POLLNVAL) 	closeConnection(i);
		else if (_pollFds[i].revents & POLLERR) 	cout << "socket fd " << _pollFds[i].fd << " ###########poll error! (POLLERR event) needs handler" << endl;	// tmp
	}	
	  													 		
}

void TCPServer::newConnection(int idx)
{
	struct pollfd	new_pollfd;
	t_socket		new_socket;
	socklen_t		socket_len;

	// memset(&new_socket, 0, sizeof(t_socket));
	new_socket.socket_address_len = sizeof(new_socket.socket_info);
	new_pollfd.fd = accept(_pollFds[idx].fd, (sockaddr *)&new_socket.socket_info, &socket_len);	
	if (new_pollfd.fd == -1) {
		exitWithError("ERROR: accept() (TCPServer::newConnection");
	}
	new_socket.socket_address_len = socket_len;

	setFileDescrOptions(new_pollfd.fd);				
	new_pollfd.events = POLLIN;
	new_socket.config_idx = idx;
	_pollFds.push_back(new_pollfd);
	_socketInfo.push_back(new_socket);

	// std::cout << "Server accepted incoming connection from ADDRESS: "
	// 		<< inet_ntoa(new_socket.socket_info.sin_addr) << "; PORT: " 
	// 		<< ntohs(new_socket.socket_info.sin_port) << "; Socket fd: " << _pollFds.back().fd << std::endl;
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

// idx geeft aan welke socket in de vector een POLLIN (read activity) heeft
// Verder ontvangt receive request data op een socket en maakt (evt) een response aan.
// Als er een response is zal de socket events op POLLOUT worden gezet, zodat in de volgende loop 
// poll() zal aangeven dat er data verstuurd kan worden. 
void TCPServer::receiveRequest(int idx)
{
	unsigned int buffer_size = _configList[_socketInfo[idx].config_idx]->getClientMaxBodySize();
	char * buff = (char *)calloc(buffer_size + 1, sizeof(char));
	// if (!buff)
	// 	do iets

	int bytes_received = read(_pollFds[idx].fd, buff, buffer_size);	
	if (bytes_received <= 0) {
		if (bytes_received < 0)
			std::cout << "Recv() error on socket fd " << _pollFds[idx].fd << " in TCPServer::startPolling()" << std::endl;		// error
		std::cout << "Socket fd " << _pollFds[idx].fd << " closed their connection." << std::endl;
		closeConnection(idx);
		return ;
	}
	string buffer = buff;
	_request.initRequest(buffer);

	class Response respons(_request, *_configList[_socketInfo[idx].config_idx]);

	_pollFds[idx].events = POLLOUT;
	// _pollFds[idx].events = POLLIN | POLLOUT;
	log_receive(_request.getMethod() + " " + _request.getURI() + " " + _request.getHTTPVersion());
}

// idx geeft aan welke socket in de vector een POLLOUT (write activity) heeft
void TCPServer::sendResponse(int idx)
{
	size_t		bytes_send;
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
			cout << "Zero bytes send. Need handler?" << endl;	// tmp
		}
	}
	_socketInfo[idx].server_message.erase(0, bytes_send);
	if (serverMsgIsEmpty(idx)) {
		_pollFds[idx].events = POLLIN;
		closeConnection(idx); 
	}
	else {
		_pollFds[idx].events = POLLOUT;
	}	
}


void	TCPServer::setFileDescrOptions(int file_descr)
{
	int	re_use = 1;

	if (fcntl(file_descr, F_SETFL, O_NONBLOCK) == -1) 						// exceptions
		exitWithError("set file descriptor as non blocking failed\n");
	if (setsockopt(file_descr, SOL_SOCKET, SO_REUSEADDR, &re_use, sizeof(re_use)) == -1) 
		exitWithError("setsockopt reuse address error\n");					
	if (setsockopt(file_descr, SOL_SOCKET, SO_REUSEPORT, &re_use, sizeof(re_use)) == -1) 
		exitWithError("setsockopt reuse port error\n");
}


void 	TCPServer::closeServer()
{
	std::exit(0);
}

} // namespace http
