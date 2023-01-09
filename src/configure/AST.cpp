
#include "AST.hpp"

/*
** ------------------------------- CONSTRUCTOR --------------------------------
*/

AST::AST()
{
}

AST::AST( const AST& src )
{
}

/*
** -------------------------------- DESTRUCTOR --------------------------------
*/

AST::~AST()
{
}

/*
** --------------------------------- OVERLOAD ---------------------------------
*/

AST&	AST::operator=( AST const& rhs )
{
	if ( this != &rhs )
	{
		this->_value = rhs.getValue();
	}
	return *this;
}

std::ostream&	operator<<( std::ostream& o, AST const& i )
{
	o << i.getValue();
	return o;
}
