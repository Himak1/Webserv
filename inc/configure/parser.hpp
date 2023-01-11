
#ifndef PARSER_HPP
# define PARSER_HPP

#include <list>

class Token;
class Node;

typedef std::list<Token*>	TokenList;

std::list<Node*>	parser( TokenList tokenList );







// class	Visitor
// {
// 	public:
// 		visit();
// 	private:
// };

#endif