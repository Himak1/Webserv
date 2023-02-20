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
	// server.startListen();
	return 0;
}
