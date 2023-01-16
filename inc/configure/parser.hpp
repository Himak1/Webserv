
#ifndef PARSER_HPP
# define PARSER_HPP

# include <list>
# include "tokenizer.hpp"

class Node;

typedef std::list<Node*>	NodeList;

Node*	parser( TokenList tList );

Node*	parseLocation( TokenList::iterator& pos, const TokenList::iterator& end );

void    parseSingleParameter( TokenList::iterator& pos, const TokenList::iterator& end );


#endif