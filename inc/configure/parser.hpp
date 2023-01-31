
#ifndef PARSER_HPP
# define PARSER_HPP

# include <list>
# include "tokenizer.hpp"

class Node;

typedef std::list<Node*>	NodeList;

Node*	parser( TokenList tList );

bool	accept( TokenList::iterator& currentToken, const TokenList::iterator& ending, int expected_token );
Node*	deleteNewNode( Node* newNode );

Node*	parseLocation( TokenList::iterator& currentToken, const TokenList::iterator& ending );
Node*	parseLocationPath( TokenList::iterator& currentToken, const TokenList::iterator& ending );
Node*   parseAlias( TokenList::iterator& currentToken, const TokenList::iterator& ending );
Node*	parseRoot( TokenList::iterator& currentToken, const TokenList::iterator& ending );
Node*	parseAllowedMethods( TokenList::iterator& currentToken, const TokenList::iterator& ending );
Node*	parseCgiPass( TokenList::iterator& currentToken, const TokenList::iterator& ending );

void    parseSingleParameter( TokenList::iterator& currentToken, const TokenList::iterator& ending );


#endif
