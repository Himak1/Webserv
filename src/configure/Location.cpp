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
	for (int i = 0; i < 3; i++)
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

