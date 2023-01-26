
#ifndef PARSER_HPP
# define PARSER_HPP

# include <list>
# include "tokenizer.hpp"

class Node;

typedef std::list<Node*>	NodeList;

Node*	parser( TokenList tList );

bool	accept( TokenList::iterator& pos, int expected_token );
Node*	deleteNewNode( Node* newNode );

Node*	parseLocation( TokenList::iterator& pos, const TokenList::iterator& ending );
Node*	parseLocationPath( TokenList::iterator& pos, const TokenList::iterator& ending );
Node*   parseLocationAlias( TokenList::iterator& pos, const TokenList::iterator& ending );

void    parseSingleParameter( TokenList::iterator& pos, const TokenList::iterator& ending );


#endif