#include "AConfig.hpp"
#include "Configuration.hpp"
#include "Location.hpp"
#include "Node.hpp"
#include "parsing/tokenizer.hpp"
#include "parsing/TokenStream.hpp"
#include "parsing/parser.hpp"
#include <iostream>
#include <fstream>

// ------------------------------------------------------------------------ //
//							Constructors & Destructor						//
// ------------------------------------------------------------------------ //

Configuration::Configuration( Node* serverNode )
{
	try {
		navigateNode(serverNode);
	}
	catch (std::exception& e) {
		throw;
	}
}

/* Configuration::Configuration(const Configuration &src) */
/* { */
/* 	*this = src; */
/* } */

Configuration::~Configuration()
{
}

/* Configuration &Configuration::operator = (const Configuration &src) */
/* { */
/* 	if (this != &src) { */
/* 		this->_pathWebsite = src._pathWebsite; */
/* 		this->_ipAddress = src._ipAddress; */
/* 		this->_port = src._port; */
/* 		this->_errorPage404 = src._errorPage404; */
/* 	} */
/* 	return (*this); */
/* } */

// ------------------------------------------------------------------------ //
//									Accessors								//
// ------------------------------------------------------------------------ //

std::string 		Configuration::getIP() const
{
	return (this->_ipAddress);
}

unsigned int	Configuration::getPort() const
{
	return (_port);
}

unsigned int	Configuration::getClientMaxBodySize() const
{
	return (_clientMaxBodySize);
}


// ------------------------------------------------------------------------ //
//									Methods									//
// ------------------------------------------------------------------------ //

void	Configuration::navigateNode( Node* serverNode )
{
	for (NodeList::const_iterator i = serverNode->getChildrenBegin(); i != serverNode->getChildrenEnd(); ++i) {
		try {
			switch ((*i)->getNodeType()) {
				case N_SERVER_NAME:
					_host = convertNodeToString(*i);
					break;
				case N_LISTEN:
					_port = convertNodeToUInt(*i);
					break;
				case N_CLIENT_MAX_BODY:
					_clientMaxBodySize = convertNodeToUInt(*i);
					break;
				case N_LOCATION:
					locations.push_back(new Location(*i));
					break;
			}
		}
		catch (std::exception& e){
			throw;
		}
	}
}

// ------------------------------------------------------------------------ //
//									External Functions						//
// ------------------------------------------------------------------------ //

static std::list<Configuration*>	createConfigurations(std::list<Configuration*>& serverConfigs, Node* ast)
{
	for (NodeList::const_iterator i = ast->getChildrenBegin(); i != ast->getChildrenEnd(); ++i) {
		try {
			serverConfigs.push_back(new Configuration(*i));
		}
		catch (std::exception& e) {
			std::cout << "invalid values were found" << std::endl;
			throw std::exception();
		}
	}
	return (serverConfigs);
}

std::list<Configuration*>	parseAndCreateConfigurations(int argc, char **argv)
{
	std::ifstream	configFile;

	if (argc >= 2)
		configFile.open(argv[1]);
	else
		configFile.open("default.conf");
	if (!configFile) {
		std::cerr << "invalid config file" << std::endl;
	}

	TokenStream	tokens = tokenizer(configFile);
	Node*		ast = parser(tokens);

	std::list<Configuration*>	serverConfigs;
	if (!ast)
		return (serverConfigs);
	return (serverConfigs);
}
