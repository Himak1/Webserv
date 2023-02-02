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

const Token*	TokenStream::getToken() const
{
	return (*_currentToken);
}

bool	TokenStream::moveToNextToken()
{
	if (_currentToken == _tokenList.end())
		return (false);
	++_currentToken;
	return (true);
}

bool	TokenStream::isEmpty() const
{
	return (_currentToken == _tokenList.end());
}
