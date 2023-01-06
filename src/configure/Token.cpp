
#include "Token.hpp"

/*
** ------------------------------- CONSTRUCTOR --------------------------------
*/

Token::Token()
	: _tokenType(-1), _token("dada")
{
}

Token::Token( int type, std::string tokenString )
	: _tokenType(type), _token(tokenString)
{
}

Token::Token( const Token& src )
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
	if ( this != &rhs )
	{
		this->_tokenType = rhs.getTokenType();
	}
	return *this;
}

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
	std::cout << "in gettoken: " << this->_token << std::endl;
	return (_token);
}

/*
** --------------------------------- ACCESSOR ---------------------------------
*/

 
