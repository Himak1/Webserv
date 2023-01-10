
#ifndef PARSER_HPP
# define PARSER_HPP

#include <list>

class Token;
class Node;

std::list<Node*>	parser( std::list<Token*> tokenList );

class	Visitor
{
	public:
		visit();
	private:
};

#endif