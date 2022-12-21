#include "TcpServer.hpp"
#include "Configuration.hpp"
#include <main.hpp>

int main(int argc, char **argv)
{
	using namespace http;

	class Configuration configuration;
	if (argc == 2)
		configuration.parseConfiguration(argv[1]);

	TcpServer server = TcpServer(configuration);
	server.startListen();

	return 0;
}
