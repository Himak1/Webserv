#include "AConfig.hpp"
#include "Configuration.hpp"
#include "Location.hpp"
#include "Node.hpp"
#include "../defines.hpp"
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
	: _host("localhost"), _port(STANDARD_PORT_NO)
{
	try {
		navigateNode(serverNode);
	}
	catch (exception& e) {
		for (list<Location*>::iterator i = locations.begin(); i != locations.end(); ++i) {
			delete *i;
		}
		throw;
	}
	if (locations.empty())
		throw runtime_error("No location block specified");
}

Configuration::~Configuration()
{
	for (list<Location*>::iterator i = locations.begin(); i != locations.end(); ++i) {
		delete *i;
	}
}

//----------------------------------Accessors-------------------------------//

string 		Configuration::getHost() const
{
	return (this->_host);
}

unsigned int	Configuration::getPort() const
{
	return (_port);
}



//----------------------------------Methods---------------------------------//


//----------------------------------External Functions----------------------//

ostream&	operator<<( ostream& o, const Configuration& config )
{
	o	<< "host: " << config.getHost() << '\n'
		<< "port: " << config.getPort() << '\n'
		<< (AConfig&)config << "\n\n";
	for (list<Location*>::const_iterator i = config.locations.begin(); i != config.locations.end(); ++i)
	{
		o	<< "----location----" << '\n'
			<< **i << '\n';
	}
	return (o);
}

