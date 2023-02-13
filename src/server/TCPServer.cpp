# include "TcpServer.hpp"
# include "../response/Response.hpp"
# include "../utils/strings.hpp"
 
# include <sys/socket.h>
# include <errno.h>
# include <poll.h>
# include <fcntl.h>
# include <iostream>
# include <sstream>
# include <unistd.h>
# include <fstream>
# include <string.h>

/////////// TOOO DOOOOO
/*

	Sockets moeten goed closen. Dit zou er voor meoten zorgen dat 'siege -b localhost:8000' werkt?


	siege -c100 -f siege_url.txt      100 concurrent users die website uit de file bezoeken

	fix vastlopers: 

unable to write to socket sock.c:733: Broken pipe
[error] socket: read error Connection reset by peer sock.c:635: Connection reset by peer
[error] socket: unable to connect sock.c:282: Connection refused


socket fd 0 ##########invalid! (POLLNVAL event) needs handler, voor nu callt deze de closeConnection()

*/




/* #################################################################################################### */
/* #################################################################################################### */	//		TMP		
/* #################################################################################################### */	//		TMP
/* ###########																				###########	*/	//		TMP
/* ###########																				###########	*/	//		TMP
/* ###########																				###########	*/	//		TMP
/* ###########		Voordat we runnen moet het max aantal open file descriptors van 		###########	*/	//		TMP
/* ###########		het OS gecheckt worden (wij hebben per connectie een file descriptor 	###########	*/	//		TMP
/* ###########		nodig).																 	###########	*/	//		TMP
/* ###########																				###########	*/	//		TMP
/* ###########		Dit doe je door in terminal 'ulimit -n' in te typen.					###########	*/	//		TMP
/* ###########																				###########	*/	//		TMP
/* ###########		Om de limiet te verhogen type je: 'ulimit -n X'	met X voor aantal fds	###########	*/	//		TMP
/* ###########																				###########	*/	//		TMP
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

	struct pollfd	listening_pollFd;
	int				re_use = 1;
	t_socket		listening_socket;

	int test_port = 8000;			// tmp

	for (int i = 0; i < 6; i++) {								
	// for (vector<int>::itterator it = getPort().begin; it < getPort().end; it++) {	// justin merge							
		memset(&listening_socket, 0, sizeof(listening_socket));
		memset(&listening_socket, 0, sizeof(listening_pollFd));
		listening_socket.socket_info.sin_family = AF_INET;

		// listening_socket.socket_info.sin_port = htons(_config.getPort());			// justin merge
		listening_socket.socket_info.sin_port = htons(test_port++); 					// tmp
		listening_socket.socket_info.sin_addr.s_addr = inet_addr(_config.getIP().c_str());
		
		listening_socket.socket_address_len = sizeof(listening_socket.socket_info);
		_socketInfo.push_back(listening_socket);

	
		listening_pollFd.fd = socket(AF_INET, SOCK_STREAM, 0);		
		if (listening_pollFd.fd < 0) {
			exitWithError("Cannot create listening socket");
		}
		fcntl(listening_pollFd.fd, F_SETFL, O_NONBLOCK);			
		
		listening_pollFd.events = POLLIN;
		_pollFds.push_back(listening_pollFd);
		_nbListeningSockets++;
		
		if (setsockopt(listening_pollFd.fd, SOL_SOCKET, SO_REUSEPORT, &re_use, sizeof(re_use)) < 0) {
			exitWithError("setsockopt error\n");
		}	

		// int rc = bind(listening_pollFd.fd, (sockaddr *)&_socketInfo[0].socket_info, _socketInfo[0].socket_address_len);
		int rc = bind(listening_pollFd.fd, (sockaddr *)&_socketInfo[i].socket_info, _socketInfo[i].socket_address_len);
		if (rc < 0) {
			exitWithError("Cannot bind() socket to address");
		}
	}
}

void TcpServer::startListen()
{
	int	poll_count;
	int i;

	for ( i = 0; i < _nbListeningSockets; i++) {
		if (listen(_pollFds[i].fd, QUEU_LIMIT_LISTEN) < 0)			
			exitWithError("Socket listen failed");
		// logStartupMessage(_socketInfo[i].socket_info);
	}

	while (_isServerRunning) {
		poll_count = poll (&_pollFds[0], _pollFds.size(), 100);		
		if (poll_count == -1) {
			exitWithError("Poll count = negative (TcpServer::startListen()");
		}
		lookupActiveSocket();
	}	
}

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
		if 	    (_pollFds[i].revents == 0)			continue;
		else if (_pollFds[i].revents & POLLIN) 		receiveRequest(i);
		else if (_pollFds[i].revents & POLLOUT) 	sendResponse(i);
		else if (_pollFds[i].revents & POLLHUP) 	closeConnection(i);			// tmp?
		else if (_pollFds[i].revents & POLLNVAL) {	
		cout << i << " revents = " << _pollFds[i].revents <<  " ##########invalid! (POLLNVAL event) needs handler" << endl; // closeConnection(i);
		}
		else if (_pollFds[i].revents & POLLERR) 	cout << "socket fd " << _pollFds[i].fd << " ###########poll error! (POLLERR event) needs handler" << endl;
	}	
	  													 		
}

/*


           SO_DEBUG        enables recording of debugging information
           SO_REUSEADDR    enables local address reuse
           SO_REUSEPORT    enables duplicate address and port bindings
           SO_KEEPALIVE    enables keep connections alive
           SO_DONTROUTE    enables routing bypass for outgoing messages
           SO_LINGER       linger on close if data present
           SO_BROADCAST    enables permission to transmit broadcast messages
           SO_OOBINLINE    enables reception of out-of-band data in band
           SO_SNDBUF       set buffer size for output
           SO_RCVBUF       set buffer size for input
           SO_SNDLOWAT     set minimum count for output
           SO_RCVLOWAT     set minimum count for input
           SO_SNDTIMEO     set timeout value for output
           SO_RCVTIMEO     set timeout value for input
           SO_TYPE         get the type of the socket (get only)
           SO_ERROR        get and clear error on the socket (get only)
           SO_NOSIGPIPE    do not generate SIGPIPE, instead return EPIPE
           SO_NREAD        number of bytes to be read (get only)
           SO_NWRITE       number of bytes written not yet sent by the
                           protocol (get only)
           SO_LINGER_SEC   linger on close if data present with timeout in
                           seconds
*/
void TcpServer::newConnection(int idx)
{
	struct pollfd	new_pollfd;
	t_socket		new_socket;
	socklen_t		socket_len;
	int				re_use = 1;

	// memset(&new_socket, 0, sizeof(t_socket));
	new_socket.socket_address_len = sizeof(new_socket.socket_info);
	new_pollfd.fd = accept(_pollFds[idx].fd, (sockaddr *)&new_socket.socket_info, &socket_len);	
	if (new_pollfd.fd == -1) {
		exitWithError("ERROR: accept() (TcpServer::newConnection");
	}
	new_socket.socket_address_len = socket_len;

	if (setsockopt(new_pollfd.fd, SOL_SOCKET, SO_REUSEADDR, &re_use, sizeof(re_use)) < 0) {		// tmp
		exitWithError("setsockopt reuse address error\n");					
	}
	if (setsockopt(new_pollfd.fd, SOL_SOCKET, SO_REUSEPORT, &re_use, sizeof(re_use)) < 0) {		// tmp
		exitWithError("setsockopt reuse port error\n");
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
		if (bytes_received < 0)
			std::cout << "Recv() error on socket fd " << _pollFds[idx].fd << " in TcpServer::startListen()" << std::endl;		
		// else
		// 	std::cout << "Socket fd " << _pollFds[idx].fd << " closed their connection." << std::endl;
		closeConnection(idx);
		return ;
	} 
		
	_request.initRequest(std::string(buff));
	
	class Response respons(_request, _config);

	_pollFds[idx].events = POLLOUT;
	// _pollFds[idx].events = POLLIN | POLLOUT;
	std::ostringstream ss;
	ss	<< "Received request: Method = " << _request.getMethod()
		<< " URI = " 					 << _request.getURI()
		<< " HTTP Version = " 			 << _request.getHTTPVersion();
	log(ss.str());
}

// idx geeft aan welke socket in de vector een POLLOUT (write activity) heeft
void TcpServer::sendResponse(int idx)
{
	size_t		bytes_send;


	class Response respons(_request, _config);

	if (_socketInfo[idx].server_message.empty())
		_socketInfo[idx].server_message = respons.getMessage();

	// if (_socketInfo[idx].server_message == "1") {	// jbe
	// 	closeConnection(idx);
	// 	return;
	// }

	// bytes_send = send(_pollFds[idx].fd, _socketInfo[idx].server_message.c_str(), _socketInfo[idx].server_message.size(), 0);
	bytes_send = write(_pollFds[idx].fd, _socketInfo[idx].server_message.c_str(), _socketInfo[idx].server_message.size());
	if (bytes_send < 0)
		std::cout << "Send error in TcpServer::sendResponse()" << std::endl;

	// if (bytes_send == (long)_socketInfo[idx].server_message.size())
	// 	log(_socketInfo[idx].server_message.substr(0, _socketInfo[idx].server_message.find('\n')));
	// else
	// 	log("Error sending response to client");


	_socketInfo[idx].server_message.erase(0, bytes_send);

	if (_socketInfo[idx].server_message.empty())
		_pollFds[idx].events = POLLIN;
		// closeConnection(idx); 
	else {
		_pollFds[idx].events = POLLOUT;
	}	


}

void TcpServer::closeServer()
{
	exit(0);
}

} // namespace http
