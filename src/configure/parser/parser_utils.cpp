#include "parser.hpp"
#include "Token.hpp"
#include "TokenStream.hpp"
#include "Node.hpp"

bool	acceptAndCreateTerminal( TokenStream& tokensToParse, Node* node)
{
	if (expect(tokensToParse, T_STRING))
	{
		node->addChild(new Node(TERMINAL, tokensToParse.getTokenString()));
		tokensToParse.moveToNextToken();
		return (true);
	}
	return (false);
}

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
