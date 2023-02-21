#include "AConfig.hpp"
#include "Configuration.hpp"
#include "Location.hpp"
#include "Node.hpp"
#include "parser/tokenizer.hpp"
#include "parser/TokenStream.hpp"
#include "parser/parser.hpp"
#include <iostream>
#include <fstream>
#include <vector>

// ------------------------------------------------------------------------ //
//							Constructors & Destructor						//
// ------------------------------------------------------------------------ //

Configuration::Configuration( Node* serverNode )
	: _host("localhost"), _port(80), _clientMaxBodySize(2)
{
	try {
		navigateNode(serverNode);
	}
	catch (std::exception& e) {
		throw;
	}
}

Configuration::~Configuration()
{
}

//----------------------------------Accessors-------------------------------//

std::string 		Configuration::getHost() const
{
	return (this->_host);
}

unsigned int	Configuration::getPort() const
{
	return (_port);
}

unsigned int	Configuration::getClientMaxBodySize() const
{
	return (_clientMaxBodySize);
}


//----------------------------------Methods---------------------------------//

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
				case N_ROOT:
					_root = convertNodeToString(*i);
					break;
				case N_INDEX:
					convertIndexFiles(*i);
					break;
				case N_ERROR_PAGE:
					convertErrorPage(*i);
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

//----------------------------------External Functions----------------------//

std::ostream&	operator<<( std::ostream& o, const Configuration& config )
{
	o	<< "host: " << config.getHost() << '\n'
		<< "port: " << config.getPort() << '\n'
		<< "client max body size: " << config.getClientMaxBodySize() << '\n'
		<< (AConfig&)config << "\n\n";
	for (std::list<Location*>::const_iterator i = config.locations.begin(); i != config.locations.end(); ++i)
	{
		o	<< "----location----" << '\n'
			<< **i << '\n';
	}
	return (o);
}

