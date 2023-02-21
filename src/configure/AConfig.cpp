#include <cstdlib>
#include "AConfig.hpp"
#include "Node.hpp"

//	ErrorPage Constructors & Destructors

ErrorPage::ErrorPage( int codeArg, std::string pageArg )
	: code(codeArg), page(pageArg)
{
}

ErrorPage::ErrorPage( const ErrorPage& src )
	: code(src.code), page(src.page)
{
}

ErrorPage::~ErrorPage()
{
}

//	AConfig Constructors & Destructors	//

AConfig::AConfig()
{
}

AConfig::~AConfig()
{
}

//	Overloads	//

std::ostream&	operator<<( std::ostream& o, const AConfig& config )
{
	o << "index files: ";
	for (std::list<std::string>::const_iterator i = config.indexFiles.begin(); i != config.indexFiles.end(); ++i)
	{
		o << *i << " ";
	}
	o << '\n' << "root: " << config.getRoot();
	return o;
}

//	Public Methods	//

std::string	AConfig::getRoot() const
{
	return (_root);
}

const std::string&	AConfig::getErrorPage( int errorCode ) const
{
	std::list<ErrorPage>::const_iterator i = _errorPages.begin();

	while (i != _errorPages.end())
	{
		if ((*i).code == errorCode)
			return ((*i).page);
		++i;
	}
	throw std::exception();
}

int	AConfig::getRedirect() const
{
	return (_redirectCode);
}

std::string	AConfig::getRedirectURI() const
{
	return (_redirectURI);
}

//	Protected Methods	//

std::string	AConfig::convertNodeToString( Node* node )
{
	NodeList::const_iterator i = node->getChildrenBegin();

	return ((*i)->getTerminal());
}

unsigned int	AConfig::convertNodeToUInt( Node* node )
{
	NodeList::const_iterator	i = node->getChildrenBegin();
	std::string					numberString;
	unsigned int				output;

	numberString = (*i)->getTerminal();
	output = strtoul(numberString.c_str(), NULL, 10);
	if (output == 0 && numberString != "0")
		throw std::exception();
	return (output);
}

void	AConfig::convertIndexFiles( Node* node )
{
	NodeList::const_iterator i = node->getChildrenBegin();

	while (i != node->getChildrenEnd())
	{
		indexFiles.push_back((*i)->getTerminal());
		++i;
	}
}

void	AConfig::convertErrorPage( Node* node )
{
	NodeList::const_iterator i = node->getChildrenBegin();

	unsigned int code = strtoul((*i)->getTerminal().c_str(), NULL, 10);
	if (code == 0 && (*i)->getTerminal()[0] != '0')
		throw std::exception();
	++i;
	std::string	page = (*i)->getTerminal();
	_errorPages.push_back(ErrorPage(code, page));
}

void	AConfig::convertReturn( Node* node )
{
	NodeList::const_iterator	i = node->getChildrenBegin();
	std::string					numberString;
	unsigned int				output;

	_redirectCode = strtoul((*i)->getTerminal().c_str(), NULL, 10);
	if (_redirectCode == 0 && (*i)->getTerminal()[0] != '0')
	{
		std::cerr << "DADADADADADDADA" << std::endl;
		throw std::exception();
	}
	++i;
	std::string	page = (*i)->getTerminal();
}

