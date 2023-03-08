#include <exception>
#include <algorithm>
#include <sys/stat.h>
#include "Location.hpp"
#include "Node.hpp"
#include <algorithm>

void	Location::convertCgiPass( Node* cgiPass )
{
	struct stat					buf;
	NodeList::const_iterator	i = cgiPass->getChildrenBegin();

	_cgiExtension = (*i)->getTerminal();
	_cgiPath = (*++i)->getTerminal();
	if (_cgiExtension != ".php" && _cgiExtension != ".py")
		throw runtime_error("invalid CGI extension");
	if (stat(_cgiPath.c_str(), &buf) != 0)
		throw runtime_error("invalid cgi path");
}

void	Location::convertAcceptedMethods( Node* allowedMethods )
{
	NodeList::const_iterator	iter = allowedMethods->getChildrenBegin();
	string					supportedMethods[3] = {"GET", "POST", "DELETE"};
	string*				ptr;
	int							i = 0; 
  
	while (iter != allowedMethods->getChildrenEnd())
	{
		ptr = find(supportedMethods, supportedMethods + 3, (*iter)->getTerminal());
		if (ptr == supportedMethods + 3)
			throw runtime_error("unsupported HTTP method in configuration file");
		_acceptedMethods[i] = (*iter)->getTerminal();
		++iter;
		i++;
	} 
}

void	Location::convertAutoIndex( Node* autoIndex )
{
	string	boolString = (*autoIndex->getChildrenBegin())->getTerminal();

	if (boolString == "on")
		_autoIndex = true;
	else if (boolString == "off")
		_autoIndex = false;
	else
		throw runtime_error("auto_index is not equal to 'on' or 'off'");
}

void	Location::convertLocation( Node* location )
{
	for (NodeList::const_iterator i = location->getChildrenBegin(); i != location->getChildrenEnd(); ++i) {
		switch ((*i)->getNodeType())
		{
			case TERMINAL:
				_path = (*i)->getTerminal();
				break;
			case N_ROOT:
				_root = convertNodeToString(*i);
				break;
			case N_ALIAS:
				_alias = convertNodeToString(*i);
				break;
			case N_CGI_PASS:
				convertCgiPass(*i);
				break;
			case N_ALLOWED_METHODS:
				convertAcceptedMethods(*i);
				break;
			case N_AUTOINDEX:
				convertAutoIndex(*i);
				break;
			case N_ERROR_PAGE:
				convertErrorPage(*i);
				break;
			case N_INDEX:
				convertIndexFiles(*i);
				break;
			case N_UPLOAD_STORE:
				convertUploadStore(*i);
				break;
			case N_CLIENT_MAX_BODY:
				convertClientMaxBodySize(*i);
				break;
			case N_RETURN:
				convertReturn(*i);
				break;
			default:
				throw exception();
		}
		if ((*i)->getNodeType() != N_ERROR_PAGE
			&& isDuplicate(location, (*i)->getNodeType()))
			throw runtime_error("duplicative directives");
	}
}
