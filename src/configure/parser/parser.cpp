
#include <list>
#include "parser.hpp"
#include "Node.hpp"

// Node*	parser( TokenList tList )
// {
// 	TokenList::iterator			currentToken = tokenList.begin();
// 	const TokenList::iterator	ending = tokenList.end();
// 	Node*						ast = new Node();

// 	while (currentToken != ending && status != -1)
// 	{
// 		ast.addNewNode();
// 		ast._nodes() = server(currentToken, ending);
// 		iter++;
// 	}
// 	return (status);
// }

// NodeList	server( TokenList::iterator& currentToken, const TokenList::iterator& end );
// {
// 	while ()
// 	{
// 		switch (iter->getTokenType())
// 		{
// 			case LOCATION:
// 				status = parseLocation();
// 				break ;
// 			case PORT:
// 				status = parsePort();
// 				break ;
// 			case SERVER_NAME:
// 				status = parseServerName();
// 				break ;
// 			default:
// 				status = -1;
// 				break ;
// 		}
// 	}
// }

Node*	deleteNewNode( Node* newNode )
{
	delete newNode;
	return (NULL);
}

bool	accept( TokenStream& tokensToParse )
{
	if (tokensToParse.
}

/* bool	accept( TokenList::iterator& currentToken, const TokenList::iterator& ending, int expected_token ) */
/* { */
/* 	if (currentToken != ending && (*currentToken)->getTokenType() == expected_token) */
/* 	{ */
/* 		++currentToken; */
/* 		return (true); */
/* 	} */
/* 	return (false); */
/* } */
