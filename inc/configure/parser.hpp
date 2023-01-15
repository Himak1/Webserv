
#ifndef PARSER_HPP
# define PARSER_HPP

#include <list>

class Token;
class Node;

typedef std::list<Node*>	NodeList;

Node*	parser( TokenList tList );

Node*	parseLocation( TokenList tList, TokenList& pos );

#endif