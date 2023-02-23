#include "AConfig.hpp"
#include "Configuration.hpp"
#include "Location.hpp"
#include "Node.hpp"
#include <stdexcept>

void	Configuration::convertPort( Node* listen )
{
	_port = convertNodeToUInt(listen);
	if (_port > 65535)
		throw std::runtime_error("ERROR: invalid port number in configuration file");
}
