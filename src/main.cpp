#include "server/TCPServer.hpp"
#include "configure/Configuration.hpp"
#include <fstream>
#include <iostream>


int main(int argc, char **argv)
{
	using namespace http;

	//Configuration*	configuration = initializeConfiguration(argc, argv);
	std::list<Configuration*>	serverConfigs = parseAndCreateConfigurations(argc, argv);

	TCPServer server(*serverConfigs.front());
	server.startListen();
	return 0;
}
