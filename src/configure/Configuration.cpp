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
Configuration::Configuration() {}

Configuration::Configuration( Node* serverNode )
	: _host("localhost"), _port(80)
{
	try {
		navigateNode(serverNode);
	}
	catch (std::exception& e) {
		for (std::list<Location*>::iterator i = locations.begin(); i != locations.end(); ++i) {
			delete *i;
		}
		throw;
	}
}

Configuration::Configuration(Configuration &src) { *this = src; }

Configuration &Configuration::operator = (Configuration &src)
{
	if (this != &src) {
		this->_host = src._host;
		this->_port = src._port;
		this->locations = src.locations;
	}
	return (*this);
}

Configuration::~Configuration()
{
	// for (std::list<Location*>::iterator i = locations.begin(); i != locations.end(); ++i) {
		// delete *i;
	// }
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

