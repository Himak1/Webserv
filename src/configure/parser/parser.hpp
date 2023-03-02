#ifndef PARSER_HPP
# define PARSER_HPP

# include <list>
# define HTTP_METHODS 3

class TokenStream;
class Node;

Node*	parser( TokenStream& tokensToParse );

Node*	parseServer( TokenStream& tokensToParse );
Node*	parseServerName( TokenStream& tokensToParse );
Node*	parseListen( TokenStream& tokensToParse );
Node*	parseIndex( TokenStream& tokensToParse );
Node*	parseClientMaxBodySize( TokenStream& tokensToParse );
Node*	parseErrorPage( TokenStream& tokensToParse );
Node*	parseUploadStore( TokenStream& tokensToParse );

Node*	parseLocation( TokenStream& tokensToParse );
Node*	parseLocationPath( TokenStream& tokensToParse );
Node*   parseAlias( TokenStream& tokensToParse );
Node*	parseRoot( TokenStream& tokensToParse );
Node*	parseAllowedMethods( TokenStream& tokensToParse );
Node*	parseCgiPass( TokenStream& tokensToParse );
Node*	parseAutoIndex( TokenStream& tokensToParse );
Node*	parseReturn( TokenStream& tokensToParse );

bool	blockIsOpen( TokenStream& tokensToParse, int& status );
void	printIncompleteBlock();
void	printUnexpectedChar( TokenStream& tokensToParse );
bool	accept( TokenStream& tokensToParse, int expected_token );
bool	expect( TokenStream& tokensToParse, int expected_token );
bool	acceptAndCreateTerminal( TokenStream& tokensToParse, Node* node );
void	acceptAndCreateNewNode( TokenStream& tokensToParse, Node* node );
Node*	deleteNewNode( Node* newNode );

#endif
