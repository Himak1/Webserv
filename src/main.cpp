#include "server/TCPServer.hpp"
#include "configure/configure.hpp"
#include <fstream>
#include <iostream>
#include <cstdlib>

using namespace std;

void	failed_allocation_handler()
{
	cerr << "ERROR: Memory allocation failed. Terminating program" << endl;
	exit(1);
}

int main(int argc, char **argv)
{
	using namespace http;

	set_new_handler(&failed_allocation_handler);
	
	vector<Configuration*>	serverConfigs = initializeConfigurations(argc, argv);
	TCPServer server(serverConfigs);

	return 0;
}
