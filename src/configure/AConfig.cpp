#include "AConfig.hpp"
#include "Node.hpp"

ErrorPage::ErrorPage( int codeArg, std::string pageArg )
	: code(codeArg), page(pageArg)
{
}

ErrorPage::~ErrorPage()
{
}

// -----------

AConfig::AConfig()
{
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
