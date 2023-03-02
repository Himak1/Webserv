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

Token::Token( Token const& src )
{
	*this = src;
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

Token&	Token::operator=( Token const& rhs )
{
	_tokenType = rhs._tokenType;
	_token = rhs._token;
	_lineNumber = rhs._lineNumber;
	return *this;
}

std::ostream&	operator<<( std::ostream& o, Token const& i )
{
	o	<< "Token: " << i.getToken() << '\n'
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

std::string	Token::getToken() const
{
	return (_token);
}

int	Token::getLineNumber() const
{
	return (_lineNumber);
}
