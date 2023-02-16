#include "server/TCPServer.hpp"
#include "configure/configure.hpp"
#include <fstream>
#include <iostream>

void	failed_allocation_handler()
{
	std::cerr << "ERROR: Memory allocation failed. Terminating program" << std::endl;
	std::terminate();
}

int main(int argc, char **argv)
{
	using namespace http;
	std::set_new_handler(&failed_allocation_handler);
	
	std::list<Configuration*>	serverConfigs = initializeConfigurations(argc, argv);
	std::cout << *serverConfigs.front() << std::endl;

	TCPServer server(*serverConfigs.front());
	server.startListen();
	return 0;
}
