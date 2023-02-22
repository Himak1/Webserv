#include "server/TCPServer.hpp"
#include "configure/configure.hpp"
#include <fstream>
#include <iostream>

void	failed_allocation_handler()
{
	std::cerr << "ERROR: Memory allocation failed. Terminating program" << std::endl;
	std::exit(1);
}

int main(int argc, char **argv)
{
	using namespace http;

	std::set_new_handler(&failed_allocation_handler);
	
	std::vector<Configuration*>	serverConfigs = initializeConfigurations(argc, argv);
	std::cout << *serverConfigs.front() << std::endl;


	// try {
		TCPServer server(serverConfigs);
	// } catch (TCPServer::TCPServerException& e) {
	// 	std::cout << "Main try: TCPServ error caught " << e.what() << std::endl;
	// 	std::exit(1);
	// } catch (std::exception& e) {
	// 	std::cout << "Main try: Standard error caught: " << e.what() << std::endl;
	// 	std::exit(2);
	// }	



	// try	{
	// } catch std::exception 
	// {}
	// server.startPolling();
	return 0;
}
