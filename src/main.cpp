#include "server/TCPServer.hpp"
#include "configure/Configuration.hpp"
#include <fstream>
#include <iostream>


int main(int argc, char **argv)
{
	using namespace http;

	std::vector<Configuration*>	serverConfigs;
	try {
		serverConfigs = parseAndCreateConfigurations(argc, argv);
	}
	catch (std::exception) {
		return 1;
	}

	// TCPServer server(*serverConfigs.front());
	TCPServer server(serverConfigs);
	// server.startListen();
	return 0;
}
