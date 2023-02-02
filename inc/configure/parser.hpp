
#ifndef PARSER_HPP
# define PARSER_HPP

# include <list>
# include "tokenizer.hpp"

class Node;

Node*	parser( TokenStream& tokensToParse );

bool	accept( TokenStream& tokensToParse, int expected_token );
Node*	deleteNewNode( Node* newNode );

Node*	parseLocation( TokenStream& tokensToParse );
Node*	parseLocationPath( TokenStream& tokensToParse );
Node*   parseAlias( TokenStream& tokensToParse );
Node*	parseRoot( TokenStream& tokensToParse );
Node*	parseAllowedMethods( TokenStream& tokensToParse );
Node*	parseCgiPass( TokenStream& tokensToParse );

void    parseSingleParameter( TokenStream& tokensToParse );


#endif
