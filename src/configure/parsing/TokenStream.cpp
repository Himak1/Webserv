#include "Token.hpp"
#include "TokenStream.hpp"

TokenStream::TokenStream( std::list<Token*> tokenList )
	: _tokenList(tokenList), _currentToken(_tokenList.begin())
{
}

TokenStream::~TokenStream()
{
	for (std::list<Token*>::iterator i = _tokenList.begin(); i != _tokenList.end(); ++i)
		delete *i;
	_tokenList.clear();
}

std::string	TokenStream::getTokenString()
{
	return ((*_currentToken)->getToken());
}

int	TokenStream::getTokenType()
{
	return ((*_currentToken)->getTokenType());
}

bool	TokenStream::moveToNextToken()
{
	if (isEmpty())
		return (false);
	++_currentToken;
	return (true);
}

bool	TokenStream::isEmpty() const
{
	return (_currentToken == _tokenList.end());
}
