#include "parser.hpp"
#include "Token.hpp"
#include "TokenStream.hpp"
#include "../Node.hpp"

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
	if (expect(tokensToParse, expectedToken) == false)
	{
		return (false);
	}
	tokensToParse.moveToNextToken();
	return (true);
}

bool	expect( TokenStream& tokensToParse, int expectedToken )
{
	if (tokensToParse.isEmpty())
	{
		std::cout	<< "Syntax Error: incomplete block or statement on last line"
					<< std::endl;
		return (false);
	}
	else if (tokensToParse.getTokenType() != expectedToken)
	{
		std::cout	<< "Syntax Error: unexpected character(s) found on line"
					<< tokensToParse.getCurrentLine() << std::endl;
		return (false);
	}
	return (true);
}

/*
bool	accept( TokenStream& tokensToParse, int expectedToken )
{
	if (!tokensToParse.isEmpty() && tokensToParse.getTokenType() == expectedToken)
	{
		tokensToParse.moveToNextToken();
		return (true);
	}
	return (false);
}


bool	accept( TokenStream& tokensToParse, int expectedToken )
{
	if (tokensToParse.isEmpty())
	{
		std::cout	<< "Syntax Error: incomplete block or statement on last line"
					<< std::endl;
		return (false);
	}
	else if (tokensToParse.getTokenType() != expectedToken)
	{
		std::cout	<< "Syntax Error: unexpected character(s) found on line"
					<< tokensToParse.getCurrentLine() << std::endl;
		return (false);
	}
	tokensToParse.moveToNextToken();
	return (true);
}

bool	expect( TokenStream& tokensToParse, int expectedToken )
{
	if (!tokensToParse.isEmpty() && tokensToParse.getTokenType() == expectedToken)
	{
		return (true);
	}
	return (false);
}
*/
