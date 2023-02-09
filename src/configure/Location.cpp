#include <exception>
#include "Location.hpp"

/*
** ------------------------------- CONSTRUCTOR --------------------------------
*/

Location::Location()
	: _autoIndex(false);
{
	_acceptedMethods[GET] = false;
	_acceptedMethods[POST] = false;
	_acceptedMethods[DELETE] = false;
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

/*
** --------------------------------- METHODS ----------------------------------
*/

std::string	Location::getRoot() const
{
	return (_root);
}

std::string	Location::getPath() const
{
	return (_path);
}

std::string	Location::getDefaultFile() const
{
	return (_defaultFile);
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

bool	Location::isMethodAccepted( int httpMethod ) const
{
	if (httpMethod < 0 || httpMethod > 3)
		throw std::exception;
	return (_acceptedMethods[httpMethod]);
}

bool	Location::autoIndexingOn() const
{
	return (_autoIndex);
}

/*
** --------------------------------- ACCESSOR ---------------------------------
*/

