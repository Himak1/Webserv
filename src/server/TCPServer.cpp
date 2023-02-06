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
void	TcpServer::setUpListeningSockets()
{

	struct pollfd	listening_pollFd;
	int				re_use = 1;
	Socket 			listening_socket;


	int test_port = 8000;	// tmp variable

	/*************************************************************/
	/* Moet loopen door het aantal poorten uit config            */
	/* ports	staan in the config class, in een list of vector */
	/* we kunnen dus itteraten op de ports				     	 */
	/*************************************************************/
	// for (int i = 0; i < 6; i++) {								
	// for (vector<int>::itterator it = getPort().begin; it < getPort().end; it++) {								
		memset(&listening_socket, 0, sizeof(listening_socket));
		
		listening_socket.socket_info.sin_family = AF_INET;
		// listening_socket.socket_info.sin_port = htons(_config.getPort());

		listening_socket.socket_info.sin_port = htons(test_port++); 			// tmp
		listening_socket.socket_info.sin_addr.s_addr = inet_addr(_config.getIP().c_str());
		listening_socket.setSocketAddressLen(sizeof(listening_socket.socket_info));
		_sockets.push_back(listening_socket);

		/*************************************************************/
		/* setup van de pollfd struct					            */
		/* setsocktopt() zorgt ervoor dat we ingeval van een restart
			de socket kunnen hergebruiken                            */

		/*************************************************************/
		memset(&listening_pollFd, 0, sizeof(listening_pollFd));
		listening_pollFd.fd = socket(AF_INET, SOCK_STREAM, 0);		
		if (listening_pollFd.fd < 0) {
			exitWithError("Cannot create listening socket");
		}
		if (setsockopt(listening_pollFd.fd, SOL_SOCKET, SO_REUSEPORT, &re_use, sizeof(re_use)) < 0) {
			exitWithError("setsockopt error\n");
		}	
		fcntl(listening_pollFd.fd, F_SETFL, O_NONBLOCK);			
		listening_pollFd.events = POLLIN;
		// int rc = bind(listening_pollFd.fd, (sockaddr *)&_sockets[i].socket_info, _sockets[i].getSocketAddressLen());
		int rc = bind(listening_pollFd.fd, (sockaddr *)&_sockets[0].socket_info, _sockets[0].getSocketAddressLen());
		if (rc < 0)
		{
			exitWithError("Cannot bind() socket to address");
		}
		_pollFds.push_back(listening_pollFd);
		_nbListeningSockets++;
	// }
}

void TcpServer::startListen()
{
	int	poll_count;
	
	for (int i = 0; i < _nbListeningSockets; i++) {
		if (listen(_pollFds[i].fd, QUEU_LIMIT_LISTEN) < 0)			
			exitWithError("Socket listen failed");
		logStartupMessage(_sockets[i].socket_info);
	}
	while (_isServerRunning) {
		log("\n====== Waiting for a new connection ======\n");

		sleep(1);												// tmp

		/****************************************************************/
		/* 	Poll checkt elke socket of er iets gebeurt (read / 			*/		
		/*	write request) en houdt dit bij in de array van structs 	*/
		/*	_pollFds (door .events aan te passen) 					*/
		/*	De -1 geeft aan dat hij oneindig lang wacht op read/write 	*/
		/*	requests)   												*/
		/****************************************************************/
		poll_count = poll (&_pollFds[0], _pollFds.size(), -1);		
		if (poll_count == -1) {
			exitWithError("Poll count = negative (TcpServer::startListen()");
		}
		// cout << "Poll count = " << poll_count << endl;		// tmp   
		lookupActiveSocket();
	}	
}

// Finds the socket where poll() found activity. It first loops through the listening sockets
// to see if there is a new connection. If not it loops through the rest of the sockets to see
// it there is any write or read activity. 
void	TcpServer::lookupActiveSocket()
{
	int i = 0;

	for (; i < _nbListeningSockets; i++)
	{
		if (_pollFds[i].revents == 0)			// nothing happened on this socket
			continue ;
		else
			newClientConnection(i);	
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

	// cout << "i in lookupacitvesocket " << i << endl;							// tmp

	// cout << (_pollFds.size() - _nbListeningSockets) << endl << endl;			// tmp

	for (int j = 0; j < (_pollFds.size() - _nbListeningSockets); j++, i++) {
		if (_pollFds[i].revents == 0)
			continue;
		if (_pollFds[i].revents & POLLIN) {
			printf("revents = %i POLLIN with i = %i\n", _pollFds[i].revents, i);		
			receiveRequest(i);
		} else if (_pollFds[i].revents & POLLOUT) {
			printf("revents = %i POLLOUT with i = %i\n", _pollFds[i].revents, i);		
			sendResponse(i);
		} else if (_pollFds[i].revents & POLLHUP) {								// ??tmp?? bij hangup van client wordt revent 17. dus POLLIN wordt dan opgeroepen en die ontvangt geen bytes (en sluit de client connection)
			cout << "revents = %i " << _pollFds[i].revents << "with i = " << i << endl;
			closeClientConnection(i);
		} 
	}  													 		
}


// 	Accepts new connections. These don't need to be set to non-blocking since they inherit from the listeningsocket
// 	(which is already non-blocking)
void TcpServer::newClientConnection(int active_socket_idx)
{
	struct pollfd	new_pollfd;
	Socket			new_socket;
	int				socket_len;
	// t_socket		new_socket;

	memset(&new_pollfd, 0, sizeof(struct pollfd));
	// memset(&new_socket, 0, sizeof(t_socket));
	// new_socket.socket_address_len = sizeof(new_socket.socket_info);
	socket_len = sizeof(new_socket.socket_info);
	new_pollfd.fd = accept(_pollFds[active_socket_idx].fd, (sockaddr *)&new_socket.socket_info, (socklen_t *)&socket_len);	
	if (new_pollfd.fd == -1) {
		exitWithError("ERROR: accept() (TcpServer::newClientConnection");
	}
	new_socket.setSocketAddressLen(socket_len);
	new_pollfd.events = POLLIN;
	_pollFds.push_back(new_pollfd);
	_sockets.push_back(new_socket);

	// cout << "socket # " << _pollFds[active_socket_idx].fd << " is new" << endl;				// tmp											
	// cout << "idx = " << active_socket_idx << endl;											// tmp		
	// cout << "_socketinfo.size = " << _socketInfo.size() << endl;								// tmp					
	// cout << "_pollFds.size = " << _pollFds.size() << endl;									// tmp				
	// cout << "listeningsockets # " << _nbListeningSockets << endl;							// tmp						
	
	std::cout << "Server accepted incoming connection from ADDRESS: "
			<< inet_ntoa(new_socket.socket_info.sin_addr) << "; PORT: " 
			<< ntohs(new_socket.socket_info.sin_port) << "; Socket fd: " << _pollFds.back().fd << std::endl;
}

void	TcpServer::closeClientConnection(int idx)
{
	close(_pollFds[idx].fd);
	_pollFds.erase(_pollFds.begin() + idx);						
	_sockets.erase(_sockets.begin() + idx);


	// cout << "idx = " << close << endl;												// tmp							
	// cout << "_socketinfo.size = " << _socketInfo.size() << endl;						// tmp				
	// cout << "_pollFds.size = " << _pollFds.size() << endl;							// tmp				
	// cout << "_nbListeningsockets # " << _nbListeningSockets << endl << endl;			// tmp							
}

// idx geeft aan welke socket in de vector een POLLIN (read activity) heeft
// Verder ontvangt receive request data op een socket en maakt (evt) een response aan.
// Als er een response is zal de socket events op POLLOUT worden gezet, zodat in de volgende loop 
// poll() zal aangeven dat er data verstuurd kan worden. 
void TcpServer::receiveRequest(int idx)
{
	char	buff[BUFFER_SIZE];
	size_t	bytes_received;

	bytes_received = recv(_pollFds[idx].fd, buff, sizeof(buff), 0);		// only difference between recv and read is dat recv als laatste argument flags heeft (flag == null, dan is recv == read)
																							// evt kan je met deze flag bepaalde errors opvangen die in de socket gebeuren 


	// std::cout << "receiveRequest heeft " << bytes_received << " ontvangen: " << buff << std::endl;		// tmp

	if (bytes_received <= 0) {
		if (bytes_received == 0)
			std::cout << "Socket fd " << _pollFds[idx].fd << " closed their connection." << std::endl;
		else
			std::cout << "Recv() error on socket fd " << _pollFds[idx].fd << " in TcpServer::startListen()" << std::endl;		
		closeClientConnection(idx);
		return ;
	} 
		

	_request.initRequest(std::string(buff));
	/*
	
		Hier zou events op POLLOUT moeten indien wij data hebben om te versturen. 
		Dit betekent dat in de volgende loop van poll, poll aangeeft dat op deze socket
		write activity is zodat we dan sendResponse kunnen sturen, en vervolgens de
		servermessage daar kunnen versturen.
		
	*/
	
	

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
	std::string	message;


	class Response respons(_request, _config);

	_sockets[idx].setServerMessage(respons.getMessage());
	if (_sockets[idx].getServerMessage().empty())
		cout << "Emtpy message on idx " << idx << endl;
	else
		_pollFds[idx].events = POLLOUT;	

	// _unsendServerMessage = _socketInfo[idx].server_message;
	// message = (char *) _sockets[idx].getServerMessage().c_str();

	// cout << "msg = " << _sockets[idx].getServerMessage().c_str() << " port " << ntohs(_sockets[idx].socket_info.sin_port) << endl;

	// printf("length of message = %lu\n", ft_strlen(message));										// tmp
	// printf("message = %s\n", message);															// tmp
	// _unsendServerMessage = "testing purpose"; 													// tmp
	// cout << "sendResponse idx = " << idx << endl;												// tmp 
	// cout << "sendResponse to port # " << ntohs(_socketInfo[idx].socket_info.sin_port) << endl;	// tmp 
	// cout << "sendResponse to fd " << _pollFds[idx].fd << endl;									// tmp 

	// while (!_unsendServerMessage.empty())
	// {
	// bytes_send = send(_pollFds[idx].fd, &_unsendServerMessage, _unsendServerMessage.size(), 0);
	bytes_send = send(_pollFds[idx].fd, _sockets[idx].getServerMessage().c_str(), message.size(), 0);
	if (bytes_send < 0)
		std::cout << "Send error in TcpServer::sendResponse()" << std::endl;

	// cout << bytes_send << " bytes are send" << endl;   											// tmp

	message.erase(0, bytes_send);
	_sockets[idx].setServerMessage(message);

	// _socketInfo[idx].server_message.erase(0, bytes_send);
	// _unsendServerMessage.erase(0, bytes_send);
	// }
	

	/*************************************************************/
	/* In het geval servermessage niet compleet is verstuurd     */
	/* blijft deze socket op POLLOUT staan zodat in de volgende  */
	/* loop hij weer kan versturen								 */
	/* Als wel alles is verstuurd gaat hij op POLLIN		     */
	/*************************************************************/

	if (message.empty())
		_pollFds[idx].events = POLLIN;
}



void TcpServer::closeServer()
{
	// close(_pollFds[0].fd); 	// exit closes all fds(?)
	exit(0);
								/*
									Als we threads gaan gebruiken geen exit() meer.
									Exit() sluit dan automatisch alle threads, maar dat is
									c++11 functionaliteit. Dus in dat geval zouden we zelf 
									threads moeten sluiten en dan exit oproepen
								*/
}




} // namespace http
