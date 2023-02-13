#include "AConfig.hpp"
#include "Node.hpp"

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

// -----------

AConfig::AConfig()
{
}

AConfig::AConfig( const AConfig& src )
	: _errorPages(src._errorPages)
{
	_root = src._root;
	/* _errorPages = src._errorPages; */
}

AConfig::~AConfig()
{
}

// ------

std::string	AConfig::convertNodeToString( Node* node )
{
	NodeList::const_iterator i = node->getChildrenBegin();

	return ((*i)->getTerminal());
}

unsigned int	AConfig::convertNodeToUInt( Node* node )
{
	NodeList::const_iterator	i = node->getChildrenBegin();
	unsigned int				output = 0;

	try {
		output = std::stoul((*i)->getTerminal());
	}
	catch (std::exception& e) {
		throw;
	}
	return (output);
}
