#include "server/TCPServer.hpp"
#include "server/Configuration.hpp"

int main(int argc, char **argv)
{
	using namespace http;

	class Configuration configuration;
	if (argc == 2)
		configuration.parseConfiguration(argv[1]);

	TCPServer server(configuration);
	server.startListen();

	return 0;
}
