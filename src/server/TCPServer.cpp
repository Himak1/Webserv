#include "TcpServer.hpp"
#include "../response/Response.hpp"
#include "../utils/strings.hpp"

#include <errno.h>
#include <poll.h>
#include <fcntl.h>
#include <iostream>
#include <sstream>
#include <unistd.h>
#include <fstream>
#include <string.h>


/* #################################################################################################### */
/* #################################################################################################### */
/* #################################################################################################### */
/* ###########																				###########	*/
/* ###########																				###########	*/
/* ###########																				###########	*/
/* ###########		Voordat we runnen moet het max aantal open file descriptors van 		###########	*/
/* ###########		het OS gecheckt worden (wij hebben per connectie een file descriptor 	###########	*/
/* ###########		nodig).																 	###########	*/
/* ###########																				###########	*/
/* ###########		Dit doe je door in terminal 'ulimit -n' in te typen.					###########	*/
/* ###########																				###########	*/
/* ###########		Om de limiet te verhogen type je: 'ulimit -n X'	met X voor aantal fds	###########	*/
/* ###########																				###########	*/
/* ###########		Voor 1000 connecties heb je ca 1020+ fds nodig							###########	*/
/* ###########																				###########	*/
/* #################################################################################################### */
/* #################################################################################################### */
/* #################################################################################################### */	



#define QUEU_LIMIT_LISTEN 20

using namespace std;

namespace
{;
	const int BUFFER_SIZE = 30720;

	void log(const std::string &message)
	{
		std::cout << message << std::endl;
	}

	void logStartupMessage(struct sockaddr_in _socketAddress)
	{
		std::ostringstream ss;
		ss	<< "\n*** Listening on ADDRESS: "
			<< inet_ntoa(_socketAddress.sin_addr) 
			<< " PORT: "
			<< ntohs(_socketAddress.sin_port) 
			<< " ***\n\n";
		log(ss.str());
	}

	void exitWithError(const std::string &errorMessage)
	{
		perror(strerror(errno));
		log("ERROR: " + errorMessage);
		exit(1);
	}
}

namespace http
{;

			// CONSTRUCTOR
TcpServer::TcpServer(class Configuration configuration)
	: _config(configuration), 
	_nbListeningSockets(0)
{
	setUpListeningSockets();
	_isServerRunning = true;
	startListen();
}

			// DESTRUCTOR
TcpServer::~TcpServer()
{
	closeServer();
}

			// PRIVATE FUNCTIONS
/* setsocktopt() zorgt ervoor dat we ingeval van een restart de socket kunnen hergebruiken                            */
void	TcpServer::setUpListeningSockets()
{
	t_socket		listening_socket;
	struct pollfd	listener;
	int				re_use = 1;

	int test_port = 8000;			// tmp

	for (int i = 0; i < 6; i++) {								
	// for (vector<int>::itterator it = getPort().begin; it < getPort().end; it++) {	// justin merge							
		memset(&listening_socket, 0, sizeof(listening_socket));
		listening_socket.socket_info.sin_family = AF_INET;
		// listening_socket.socket_info.sin_port = htons(_config.getPort());			// justin merge

		listening_socket.socket_info.sin_port = htons(test_port++); 					// tmp
		listening_socket.socket_info.sin_addr.s_addr = inet_addr(_config.getIP().c_str());
		
		listening_socket.socket_address_len = sizeof(listening_socket.socket_info);
		_socketInfo.push_back(listening_socket);

	
		memset(&listener, 0, sizeof(listener));
		listener.fd = socket(AF_INET, SOCK_STREAM, 0);		
		if (listener.fd < 0) {
			exitWithError("Cannot create listening socket");
		}
		if (setsockopt(listener.fd, SOL_SOCKET, SO_REUSEPORT, &re_use, sizeof(re_use)) < 0) {
			exitWithError("setsockopt error\n");
		}	
		fcntl(listener.fd, F_SETFL, O_NONBLOCK);			
		listener.events = POLLIN;
		if (bind(listener.fd, (sockaddr *)&_socketInfo[i].socket_info, _socketInfo[i].socket_address_len) < 0) {		/// we casten van sockaddr_in -> sock_addr waarom?
			exitWithError("Cannot bind() socket to address");
		}
		_pollFds.push_back(listener);
		_nbListeningSockets++;
	}
}

void TcpServer::startListen()
{
	int	poll_count;
	
	for (int i = 0; i < _nbListeningSockets; i++) {
		if (listen(_pollFds[i].fd, QUEU_LIMIT_LISTEN) < 0)			
			exitWithError("Socket listen failed");
		logStartupMessage(_socketInfo[i].socket_info);
	}
	while (_isServerRunning) {
		log("\n====== Waiting for a new connection ======\n");
		poll_count = poll (&_pollFds[0], _pollFds.size(), -1);		
		if (poll_count == -1) {
			exitWithError("Poll count = negative (TcpServer::startListen()");
		}
		lookupActiveSocket();
	}	
}

/*	POLLERR, POLLHUP, or POLLNVAL. Deze worden altijd gecheckt door POLL, ongeacht 	*/
/*	de waarde van .events															*/
/*	POLLER = Error condition (only returned in revents; ignored in
              events).  This bit is also set for a file descriptor
              referring to the write end of a pipe when the read end has
              been closed.															*/
/*	POLLHUP = Hang up (only returned in revents; ignored in events).
              Note that when reading from a channel such as a pipe or a
              stream socket, this event merely indicates that the peer
              closed its end of the channel.  Subsequent reads from the
              channel will return 0 (end of file) only after all
              outstanding data in the channel has been consumed.					*/
/*	POLLNVAL = Invalid request: fd not open (only returned in revents;
              ignored in events).	*/
void	TcpServer::lookupActiveSocket()
{
	int i;

	for (i = 0; i < _nbListeningSockets; i++) {
		if (_pollFds[i].revents == 0)
			continue ;
		else
			newConnection(i);	
	}

	for (int j = 0; j < (_pollFds.size() - _nbListeningSockets); j++, i++) {
		if (_pollFds[i].revents == 0)
			continue;
		if (_pollFds[i].revents & POLLIN) 			receiveRequest(i);
		else if (_pollFds[i].revents & POLLOUT) 	sendResponse(i);
		else if (_pollFds[i].revents & POLLHUP) 	closeConnection(i);			// tmp?
		else if (_pollFds[i].revents & POLLNVAL) {	// tmp?
			cout << "socket fd " << _pollFds[i].fd << " ##########invalid! (POLLNVAL event) needs handler" << endl;		
		} else if (_pollFds[i].revents & POLLERR) {	// tmp?
			cout << "socket fd " << _pollFds[i].fd << " ###########poll error! (POLLERR event) needs handler" << endl;
		}	
	}  													 		
}


// 	Accepts new connections. These don't need to be set to non-blocking since they inherit from the listeningsocket
// 	(which is already non-blocking)
void TcpServer::newConnection(int active_socket_idx)
{
	struct pollfd	new_pollfd;
	t_socket		new_socket;

	memset(&new_pollfd, 0, sizeof(struct pollfd));
	memset(&new_socket, 0, sizeof(t_socket));
	new_socket.socket_address_len = sizeof(new_socket.socket_info);
	new_pollfd.fd = accept(_pollFds[active_socket_idx].fd, (sockaddr *)&new_socket.socket_info, &new_socket.socket_address_len);	// moet het laatste argument niet sizeof(socketinfo) zijn?
	if (new_pollfd.fd == -1) {
		exitWithError("ERROR: accept() (TcpServer::newConnection");
	}
	new_pollfd.events = POLLIN;
	_pollFds.push_back(new_pollfd);
	_socketInfo.push_back(new_socket);

	// std::cout << "Server accepted incoming connection from ADDRESS: "
	// 		<< inet_ntoa(new_socket.socket_info.sin_addr) << "; PORT: " 
	// 		<< ntohs(new_socket.socket_info.sin_port) << "; Socket fd: " << _pollFds.back().fd << std::endl;
}

void	TcpServer::closeConnection(int idx)
{
	close(_pollFds[idx].fd);
	_pollFds.erase(_pollFds.begin() + idx);						
	_socketInfo.erase(_socketInfo.begin() + idx);

}

// idx geeft aan welke socket in de vector een POLLIN (read activity) heeft
// Verder ontvangt receive request data op een socket en maakt (evt) een response aan.
// Als er een response is zal de socket events op POLLOUT worden gezet, zodat in de volgende loop 
// poll() zal aangeven dat er data verstuurd kan worden. 
void TcpServer::receiveRequest(int idx)
{
	char	buff[BUFFER_SIZE];
	size_t	bytes_received;

	bytes_received = recv(_pollFds[idx].fd, buff, sizeof(buff), 0);		
	if (bytes_received <= 0) {
		if (bytes_received == 0)
			std::cout << "Socket fd " << _pollFds[idx].fd << " closed their connection." << std::endl;
		else
			std::cout << "Recv() error on socket fd " << _pollFds[idx].fd << " in TcpServer::startListen()" << std::endl;		
		closeConnection(idx);
		return ;
	} 
		
	_request.initRequest(std::string(buff));
	
	class Response respons(_request, _config);

	_socketInfo[idx].server_message = respons.getMessage();
	if (_socketInfo[idx].server_message.empty())
		cout << "Emtpy message on idx " << idx << endl;
	else
		_pollFds[idx].events = POLLOUT;	

	std::ostringstream ss;
	ss	<< "Received request: Method = " << _request.getMethod()
		<< " URI = " 					 << _request.getURI()
		<< " HTTP Version = " 			 << _request.getHTTPVersion();
	log(ss.str());
}

// idx geeft aan welke socket in de vector een POLLOUT (write activity) heeft
void TcpServer::sendResponse(int idx)
{
	size_t	bytes_send;
	char	*message;

	message = (char *) _socketInfo[idx].server_message.c_str();

	bytes_send = send(_pollFds[idx].fd, message, ft_strlen(message), 0);
	if (bytes_send < 0)
		std::cout << "Send error in TcpServer::sendResponse()" << std::endl;

	_socketInfo[idx].server_message = message;
	_socketInfo[idx].server_message.erase(0, bytes_send);

	if (_socketInfo[idx].server_message.empty())
		_pollFds[idx].events = POLLIN;
}

void TcpServer::closeServer()
{
	exit(0);
}

} // namespace http
