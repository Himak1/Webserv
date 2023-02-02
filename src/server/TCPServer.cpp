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

void	TcpServer::setUpListeningSockets()
{
	t_socket		listening_socket;
	struct pollfd	listener;
	int				re_use = 1;
	int test_port = 8000;	// tmp variable

	/*************************************************************/
	/* Moet loopen door het aantal poorten uit config            */
	/* ports	staan in the config class, in een list of vector */
	/* we kunnen dus itteraten op de ports				     	 */
	/*************************************************************/
	for (int i = 0; i < 6; i++) {								
	// for (vector<int>::itterator it = getPort().begin; it < getPort().end; it++) {								
		memset(&listening_socket, 0, sizeof(listening_socket));
		listening_socket.socket_info.sin_family = AF_INET;
		// listening_socket.socket_info.sin_port = htons(_config.getPort());		// port from itterator

		listening_socket.socket_info.sin_port = htons(test_port++); 			// tmp
		listening_socket.socket_info.sin_addr.s_addr = inet_addr(_config.getIP().c_str()); 	// ook van ittertor
		listening_socket.socket_address_len = sizeof(listening_socket.socket_info);
		_socketInfo.push_back(listening_socket);

		/*************************************************************/
		/* setup van de pollfd struct					            */
		/* setsocktopt() zorgt ervoor dat we ingeval van een restart
			de socket kunnen hergebruiken                            */

		/*************************************************************/
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
		if (bind(listener.fd, (sockaddr *)&_socketInfo[i].socket_info, _socketInfo[i].socket_address_len) < 0) {
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
		lookupActiveSocket();
	}	
}

// Finds the socket where poll() found activity. It first loops through the listening sockets
// to see if there is a new connection. If not it loops through the rest of the sockets to see
// it there is any write or read activity. revents == 0 means nothing happened
void	TcpServer::lookupActiveSocket()
{
	int i = 0;

	for (; i < _nbListeningSockets; i++)
	{
		if (_pollFds[i].revents == 0)
			continue ;
		else
			newConnection(i);	
	}

/*	POLLERR, POLLHUP, or POLLNVAL. Deze worden altijd gecheckt door POLL, ongeacht 	*/
/*	de waarde van .events															*/
/*	POLLER = Error condition (only returned in revents; ignored in					*/
/*             events).  This bit is also set for a file descriptor					*/
/*              referring to the write end of a pipe when the read end has			*/	
/*              been closed.														*/
/*	POLLHUP = Hang up (only returned in revents; ignored in events).				*/				
/*            Note that when reading from a channel such as a pipe or a				*/			
/*            stream socket, this event merely indicates that the peer				*/			
/*            closed its end of the channel.  Subsequent reads from the				*/			
/*            channel will return 0 (end of file) only after all					*/			
/*            outstanding data in the channel has been consumed.					*/
/*	POLLNVAL = Invalid request: fd not open (only returned in revents;				*/			
/*              ignored in events).													*/

	for (int j = 0; j < (_pollFds.size() - _nbListeningSockets); j++, i++) {
		if (_pollFds[i].revents == 0)
			continue;
		if (_pollFds[i].revents & POLLIN) {
			receiveRequest(i);
		} else if (_pollFds[i].revents & POLLOUT) {
			sendResponse(i);
		} else if (_pollFds[i].revents & POLLHUP) {		// tmp?
			closeClientConnection(i);
		} else if (_pollFds[i].revents & POLLNVAL) {	// tmp?
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

	// cout << "socket # " << _pollFds[active_socket_idx].fd << " is new" << endl;				// tmp											
	// cout << "idx = " << active_socket_idx << endl;											// tmp		
	// cout << "_socketinfo.size = " << _socketInfo.size() << endl;								// tmp					
	// cout << "_pollFds.size = " << _pollFds.size() << endl;									// tmp				
	// cout << "listeningsockets # " << _nbListeningSockets << endl;							// tmp						
	
	// std::cout << "Server accepted incoming connection from ADDRESS: "
	// 		<< inet_ntoa(new_socket.socket_info.sin_addr) << "; PORT: " 
	// 		<< ntohs(new_socket.socket_info.sin_port) << "; Socket fd: " << _pollFds.back().fd << std::endl;
}

void	TcpServer::closeClientConnection(int idx)
{
	close(_pollFds[idx].fd);
	_pollFds.erase(_pollFds.begin() + idx);						
	_socketInfo.erase(_socketInfo.begin() + idx);


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

	// cout << "receivereq with i = " << idx << endl;

	// std::cout << "receiveRequest heeft " << bytes_received << " ontvangen: " << buff << std::endl;

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
	
	class Response respons(_request, _config);

	_socketInfo[idx].server_message = respons.getMessage();
	if (_socketInfo[idx].server_message.empty())
		cout << "Emtpy message on idx " << idx << endl;
	else
		_pollFds[idx].events = POLLOUT;	

	// if (!_request.isValidMethod())
	// 	exitWithError("Invalid method, only GET, POST and DELETED are supported");
	std::ostringstream ss;
	// ss	<< "Received request: Method = " << _request.getMethod()
	// 	<< " URI = " 					 << _request.getURI()
	// 	<< " HTTP Version = " 			 << _request.getHTTPVersion();
	// log(ss.str());
}

// idx geeft aan welke socket in de vector een POLLOUT (write activity) heeft
void TcpServer::sendResponse(int idx)
{
	size_t	bytes_send;
	char	*message;

	// _unsendServerMessage = _socketInfo[idx].server_message;
	message = (char *) _socketInfo[idx].server_message.c_str();

	// printf("length of message = %lu\n", ft_strlen(message));			// tmp
	// printf("message = %s\n", message);									// tmp


	// _unsendServerMessage = "testing purpose"; 	// tmp
	// cout << "sendResponse idx = " << idx << endl;		// tmp test
	// cout << "sendResponse to port # " << ntohs(_socketInfo[idx].socket_info.sin_port) << endl;	// tmp test
	// cout << "sendResponse to fd " << _pollFds[idx].fd << endl;				// tmp test

	// while (!_unsendServerMessage.empty())
	// {
	// bytes_send = send(_pollFds[idx].fd, &_unsendServerMessage, _unsendServerMessage.size(), 0);
	bytes_send = send(_pollFds[idx].fd, message, ft_strlen(message), 0);
	if (bytes_send < 0)
		std::cout << "Send error in TcpServer::sendResponse()" << std::endl;

	// cout << bytes_send << " bytes are send" << endl;   			// tmp

	_socketInfo[idx].server_message = message;
	_socketInfo[idx].server_message.erase(0, bytes_send);
	// _unsendServerMessage.erase(0, bytes_send);
	// }
	

	/*************************************************************/
	/* In het geval servermessage niet compleet is verstuurd     */
	/* blijft deze socket op POLLOUT staan zodat in de volgende  */
	/* loop hij weer kan versturen								 */
	/* Als wel alles is verstuurd gaat hij op POLLIN		     */
	/*************************************************************/

	if (_socketInfo[idx].server_message.empty())
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
