#include "AConfig.hpp"
#include "Configuration.hpp"
#include "Location.hpp"
#include "Node.hpp"
#include <stdexcept>

void	Configuration::convertPort( Node* listen )
{
	_port = convertNodeToUInt(listen);
	if (_port > 65535)
		throw runtime_error("invalid port number in configuration file");
}

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
				throw exception();
		}
		if ((*i)->getNodeType() != N_ERROR_PAGE
			&& (*i)->getNodeType() != N_LOCATION
			&& isDuplicate(serverNode, (*i)->getNodeType()))
			throw runtime_error("duplicative directives");
	}
}
