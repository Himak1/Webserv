
#include <list>
#include "parser.hpp"
#include "Node.hpp"
#include "TokenStream.hpp"

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

void	acceptAndCreateNewNode( TokenStream& tokensToParse, Node* node )
{
	node->addChild(new Node(TERMINAL, tokensToParse.getTokenString()));
	tokensToParse.moveToNextToken();
}

Node*	deleteNewNode( Node* newNode )
{
	delete newNode;
	return (NULL);
}

bool	accept( TokenStream& tokensToParse, int expectedToken )
{
	if (!tokensToParse.isEmpty() && tokensToParse.getTokenType() == expectedToken)
	{
		tokensToParse.moveToNextToken();
		return (true);
	}
	return (false);
}

bool	expect( TokenStream& tokensToParse, int expectedToken )
{
	if (!tokensToParse.isEmpty() && tokensToParse.getTokenType() == expectedToken)
	{
		return (true);
	}
	return (false);
}
