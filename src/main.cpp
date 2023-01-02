#include <TcpServer.hpp>
#include "configuration/Configuration.hpp"
#include <main.hpp>

int main(int argc, char **argv)
{
	using namespace http;

	class Configuration configuration;
	if (argc == 2)
		configuration.parseConfiguration(argv[1]);

	TcpServer server8000 = TcpServer(configuration);
	TcpServer server8001 = TcpServer(configuration);
	server8000.startListen();
	server8001.startListen();


	return 0;
}
