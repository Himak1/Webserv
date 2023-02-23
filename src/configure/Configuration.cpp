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
	: _host("localhost"), _port(80)
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
	for (std::list<Location*>::iterator i = locations.begin(); i != locations.end(); ++i) {
		delete *i;
	}
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



//----------------------------------Methods---------------------------------//

void	Configuration::navigateNode( Node* serverNode )
{
	for (NodeList::const_iterator i = serverNode->getChildrenBegin(); i != serverNode->getChildrenEnd(); ++i) {
		switch ((*i)->getNodeType()) {
			case N_SERVER_NAME:
				_host = convertNodeToString(*i);
				break;
			case N_LISTEN:
				convertPort(*i);
				break;
			case N_CLIENT_MAX_BODY:
				convertClientMaxBodySize(*i);
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
			case N_UPLOAD_STORE:
				convertUploadStore(*i);
				break;
			case N_RETURN:
				convertReturn(*i);
				break;
			default:
				throw std::exception();
		}
	}
}

//----------------------------------External Functions----------------------//

std::ostream&	operator<<( std::ostream& o, const Configuration& config )
{
	o	<< "host: " << config.getHost() << '\n'
		<< "port: " << config.getPort() << '\n'
		<< (AConfig&)config << "\n\n";
	for (std::list<Location*>::const_iterator i = config.locations.begin(); i != config.locations.end(); ++i)
	{
		o	<< "----location----" << '\n'
			<< **i << '\n';
	}
	return (o);
}

