#include <exception>
#include "Location.hpp"
#include "Node.hpp"

/*
** ------------------------------- CONSTRUCTOR --------------------------------
*/

Location::Location( Node* locationNode )
	: _autoIndex(false)
{
	try {
		convertLocation(locationNode);
	}
	catch (std::exception& e) {
		throw;
	}
}

/*
** -------------------------------- DESTRUCTOR --------------------------------
*/

Location::~Location()
{
}

/*
** --------------------------------- OVERLOAD ---------------------------------
*/

std::ostream&	operator<<( std::ostream& o, const Location& location )
{
	o	<< "path: " << location.getPath() << '\n'
		<< (AConfig&)location << '\n'
		<< "alias: " << location.getAlias() << '\n'
		<< "CGI Extension: " << location.getCgiExtension() << '\n'
		<< "CGI Path: " << location.getCgiPath();
	return (o);
}

/*
** --------------------------------- METHODS ----------------------------------
*/

void	Location::convertCgiPass( Node* cgiPass )
{
	NodeList::const_iterator i = cgiPass->getChildrenBegin();

	_cgiExtension = (*i)->getTerminal();
	_cgiPath = (*++i)->getTerminal();
}

void	Location::convertAcceptedMethods( Node* allowedMethods )
{
	NodeList::const_iterator iter = allowedMethods->getChildrenBegin();
	int	i = 0;

	while (iter != allowedMethods->getChildrenEnd())
	{
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
		throw std::exception();
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
			case N_RETURN:
				convertReturn(*i);
				break;
			default:
				throw std::exception();
		}
	}
}

/*
** --------------------------------- ACCESSOR ---------------------------------
*/

std::string	Location::getPath() const
{ return (_path);
}

std::string	Location::getAlias() const
{
	return (_alias);
}

std::string	Location::getCgiExtension() const
{
	return (_cgiExtension);
}

std::string	Location::getCgiPath() const
{
	return (_cgiPath);
}

bool	Location::isMethodAccepted( std::string& httpMethod ) const
{
	for (int i = 0; i < 4; i++)
	{
		if (httpMethod == _acceptedMethods[i])
			return (true);
	}
	return (false);
}

bool	Location::autoIndexingOn() const
{
	return (_autoIndex);
}

