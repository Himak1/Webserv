#include "AConfig.hpp"
#include "Configuration.hpp"
#include "Location.hpp"
#include "Node.hpp"
#include <iostream>

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

std::list<Configuration>	createConfigurations(Node* ast)
{
	std::list<Configuration>	serverConfigs;

	for (NodeList::const_iterator i = ast->getChildrenBegin(); i != ast->getChildrenEnd(); ++i) {
		try {
			serverConfigs.push_back(Configuration(*i));
		}
		catch (std::exception& e) {
			std::cout << "invalid values were found" << std::endl;
			throw std::exception();
		}
	}
	return (serverConfigs);
}
