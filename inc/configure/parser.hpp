
#ifndef PARSER_HPP
# define PARSER_HPP

# include <list>
# include "tokenizer.hpp"

class Node;

Node*	parser( TokenList tList );

bool	accept( TokenStream& tokensToParse, int expected_token );
Node*	deleteNewNode( Node* newNode );

Node*	parseLocation( TokenStream& tokensToParse );
Node*	parseLocationPath( TokenStream& TokensToParse );
Node*   parseAlias( TokenStream& tokensToParse );
Node*	parseRoot( TokenStream& tokensToParse );
Node*	parseAllowedMethods( TokenStream& tokensToParse );
Node*	parseCgiPass( TokenStream& tokensToParse );

void    parseSingleParameter( TokenStream& tokensToParse );


#endif
