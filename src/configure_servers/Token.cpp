#include "Token.hpp"

/*
** ------------------------------- CONSTRUCTOR --------------------------------
*/

Token::Token()
	: _tokenType(INVALID_TOKEN)
{
}

Token::Token( const Token& src )
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

Token&	Token::operator=( Token const& rhs )
{
	//if ( this != &rhs )
	//{
		//this->_value = rhs.getValue();
	//}
	return *this;
}

std::ostream&	operator<<( std::ostream& o, Token const& i )
{
	//o << "Value = " << i.getValue();
	return o;
}


/*
** --------------------------------- METHODS ----------------------------------
*/

int&	Token::getToken() const
{
	return (_tokenType);
}

std::string&	Token::getToken() const
{
	return (_token);
}

/*
** --------------------------------- ACCESSOR ---------------------------------
*/

 
