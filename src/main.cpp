#include "server/TcpServer.hpp"
#include "server/Configuration.hpp"


int main(int argc, char **argv)
{
	using namespace http;

	class Configuration configuration;
	if (argc == 2)
		configuration.parseConfiguration(argv[1]);

	TcpServer server8000 = TcpServer(configuration);

	return 0;
}
