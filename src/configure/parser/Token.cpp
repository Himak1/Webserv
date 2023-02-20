#include "Token.hpp"

/*
** ------------------------------- CONSTRUCTOR --------------------------------
*/

Token::Token()
	: _tokenType(-1), _token("dada")
{
}

Token::Token( int type, std::string tokenString, int lineNumber )
	: _tokenType(type), _token(tokenString), _lineNumber(lineNumber)
{
}

Token::Token( const Token& src )
	: _tokenType(src._tokenType), _token(src._token), _lineNumber(src._lineNumber)
{
}


/*
** -------------------------------- DESTRUCTOR --------------------------------
*/

Token::~Token()
{
}


/*
** --------------------------------- OVERLOAD ---------------------------------
*/

std::ostream&	operator<<( std::ostream& o, Token const& i )
{
	o << "Token: " << i.getToken() << '\n'
		<< "Type: " << i.getTokenType() << std::endl;
	return o;
}


/*
** --------------------------------- METHODS ----------------------------------
*/

int	Token::getTokenType() const
{
	return (_tokenType);
}

const std::string&	Token::getToken() const
{
	return (_token);
}

int	Token::getLineNumber() const
{
	return (_lineNumber);
}
