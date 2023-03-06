#include <cstdlib>
#include <climits>
#include <cctype>
#include <cerrno>
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
	: _uploadStore("/"), _clientMaxBodySize(UINT_MAX), _redirectCode(0)
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
	o	<< '\n' << "root: " << config.getRoot() << '\n'
		<< "client max body size: " << config.getClientMaxBodySize() << '\n'
		<< "upload store:" << config.getUploadStore() << '\n'
		<< "redirect code: " << config.getRedirect() << '\n'
		<< "redirect URI: " << config.getRedirectURI();
	return o;
}

//	Public Methods	//

std::string	AConfig::getRoot() const
{
	return (_root);
}

std::string	AConfig::getErrorPage( int errorCode ) const
{
	std::list<ErrorPage>::const_iterator i = _errorPages.begin();

	while (i != _errorPages.end())
	{
		if ((*i).code == errorCode)
			return ((*i).page);
		++i;
	}
	return ("");
}

std::string	AConfig::getUploadStore() const
{
	return (_uploadStore);
}

unsigned int	AConfig::getClientMaxBodySize() const
{
	return (_clientMaxBodySize);
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
	if ((output == 0 && numberString != "0") || errno == ERANGE || output > UINT_MAX)
		throw std::runtime_error("cannot convert parameter to number");
	for (int it = 1; it < static_cast<int>(numberString.size()); it++) {
		if (!isdigit(numberString[it])) throw std::runtime_error("cannot convert parameter to number");
	}
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
		throw std::runtime_error("cannot convert parameter to number");
	++i;
	std::string	page = (*i)->getTerminal();
	_errorPages.push_back(ErrorPage(code, page));
}

void	AConfig::convertUploadStore( Node* node )
{
	NodeList::const_iterator	i = node->getChildrenBegin();

	_uploadStore = (*i)->getTerminal();
}

void	AConfig::convertClientMaxBodySize( Node* node )
{
	_clientMaxBodySize = convertNodeToUInt(node);
	if (_clientMaxBodySize == 0)
		throw std::runtime_error("client_max_body_size cannot be set to 0");
}

void	AConfig::convertReturn( Node* node )
{
	NodeList::const_iterator	i = node->getChildrenBegin();

	_redirectCode = strtoul((*i)->getTerminal().c_str(), NULL, 10);
	if (_redirectCode == 0 && (*i)->getTerminal()[0] != '0')
		throw std::runtime_error("return parameter cannot be converted to number");
	if (_redirectCode < 199 || _redirectCode > 599)
		throw std::runtime_error("invalid return code");
	++i;
	_redirectURI = (*i)->getTerminal();
}

