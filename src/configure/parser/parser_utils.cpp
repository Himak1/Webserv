#include "parser.hpp"
#include "Token.hpp"
#include "TokenStream.hpp"
#include "../Node.hpp"

bool	blockIsOpen( TokenStream& tokensToParse, int& status )
{
	if (status == 0)
		return (false);
	if (tokensToParse.isEmpty())
	{
		status = 0;
		printIncompleteBlock();
		return (false);
	}
	else if (tokensToParse.getTokenType() == T_BRACKET_CLOSE)
		return (false);
	return (true);
}

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

void	printIncompleteBlock()
{
	std::cout	<< "Syntax Error: incomplete block or statement on last line"
				<< std::endl;
}

void	printUnexpectedChar( TokenStream& tokensToParse )
{
	std::cout	<< "Syntax Error: unexpected character(s) '"
				<< tokensToParse.getTokenString() << "' on line "
				<< tokensToParse.getCurrentLine() << std::endl;
}

bool	expect( TokenStream& tokensToParse, int expectedToken )
{
	if (tokensToParse.isEmpty())
	{
		printIncompleteBlock();
		return (false);
	}
	else if (tokensToParse.getTokenType() != expectedToken)
	{
		printUnexpectedChar(tokensToParse);
		return (false);
	}
	return (true);
}

