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

Location::Location( const Location& src )
{
	*this = src;
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

Location&	Location::operator=( Location const& src )
{
	_path = src._path;
	_alias = src._alias;
	_cgiExtension = src._cgiExtension;
	_cgiPath = src._cgiPath;
	_autoIndex = src._autoIndex;
	for (int i = 0; i < 4; i++)
	{
		_acceptedMethods[i] = src._acceptedMethods[i];
	}
}

/*
** --------------------------------- METHODS ----------------------------------
*/

void	Location::convertLocation( Node* locationNode )
{
	for (NodeList::const_iterator i = locationNode->getChildrenBegin(); ++i) {
		try {
			switch ((*i)->getNodeType())
			{
				case TERMINAL:
					_path = convertNodeToString(*i);
					break;
				case N_ROOT:
					_root = convertNodeToString(*i);
					break;
			}
		}
		catch (std::exception& e) {
		
		}
	}
}

/*
** --------------------------------- ACCESSOR ---------------------------------
*/

std::string	Location::getPath() const
{
	return (_path);
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

bool	Location::isMethodAccepted( std::string httpMethod ) const
{

	return (false);
}

bool	Location::autoIndexingOn() const
{
	return (_autoIndex);
}

