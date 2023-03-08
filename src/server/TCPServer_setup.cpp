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

static void logStartupMessage(struct sockaddr_in _socketAddress)
{
	ostringstream ss;

	ss	<< "Listening on PORT: "
		<< ntohs(_socketAddress.sin_port);
	log_receive(ss.str());
}

namespace http
{

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

bool	TCPServer::serverMsgIsEmpty(int idx)
{
	if (_socketInfo[idx].server_message.empty())
		return true;
	else
		return false;	
}

}	// namespace
