#include <exception>
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
		throw std::runtime_error("ERROR: invalid CGI extension");
	if (stat(_cgiPath.c_str(), &buf) != 0)
		throw std::runtime_error("ERROR: invalid cgi path");
}

void	Location::convertAcceptedMethods( Node* allowedMethods )
{
	NodeList::const_iterator	iter = allowedMethods->getChildrenBegin();
	std::string					supportedMethods[3] = {"GET", "POST", "DELETE"};
	std::string*				ptr;
	int							i = 0; 
  
	while (iter != allowedMethods->getChildrenEnd())
	{
		ptr = std::find(supportedMethods, supportedMethods + 3, (*iter)->getTerminal());
		if (ptr == supportedMethods + 3)
			throw std::runtime_error("ERROR: unsupported HTTP method in configuration file");
		_acceptedMethods[i] = (*iter)->getTerminal();
		++iter;
		i++;
	} 
}

void	Location::convertAutoIndex( Node* autoIndex )
{
	std::string	boolString = (*autoIndex->getChildrenBegin())->getTerminal();

	if (boolString == "on")
		_autoIndex = true;
	else if (boolString == "off")
		_autoIndex = false;
	else
		throw std::runtime_error("ERROR: auto_index is not equal to 'on' or 'off'");
}
