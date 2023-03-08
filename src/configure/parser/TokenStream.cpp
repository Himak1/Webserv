#include "TokenStream.hpp"

TokenStream::TokenStream( list<Token> tokenList )
	: _tokenList(tokenList), _currentToken(_tokenList.begin())
{
}

TokenStream::~TokenStream()
{
	_tokenList.clear();
}

string	TokenStream::getTokenString()
{
	return ((*_currentToken).getToken());
}

int	TokenStream::getTokenType()
{
	return ((*_currentToken).getTokenType());
}

int	TokenStream::getCurrentLine()
{
	return ((*_currentToken).getLineNumber());
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
