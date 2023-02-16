#include <list>
#include "parser.hpp"
#include "../Node.hpp"
#include "Token.hpp"
#include "TokenStream.hpp"

Node*	parseClientMaxBodySize( TokenStream& tokensToParse )
{
	Node*	newNode;

	tokensToParse.moveToNextToken();
	newNode = new Node(N_CLIENT_MAX_BODY);
	if (!acceptAndCreateTerminal(tokensToParse, newNode))
		return (deleteNewNode(newNode));
	if (!accept(tokensToParse, T_SEMICOLON))
		return (deleteNewNode(newNode));
	return (newNode);
}

Node*	parseIndex( TokenStream& tokensToParse )
{
	Node*	newNode;

	tokensToParse.moveToNextToken();
	newNode = new Node(N_INDEX);
	while (expect(tokensToParse, T_STRING))
	{
		acceptAndCreateTerminal(tokensToParse, newNode);
	}
	if (!accept(tokensToParse, T_SEMICOLON))
		return (deleteNewNode(newNode));
	return (newNode);
}

Node*	parseErrorPage( TokenStream& tokensToParse )
{
	Node*	newNode;

	tokensToParse.moveToNextToken();
	newNode = new Node(N_ERROR_PAGE);
	if (!acceptAndCreateTerminal(tokensToParse, newNode))
		return (deleteNewNode(newNode));
	if (!acceptAndCreateTerminal(tokensToParse, newNode))
		return (deleteNewNode(newNode));
	if (!accept(tokensToParse, T_SEMICOLON))
		return (deleteNewNode(newNode));
	return (newNode);
}

Node*	parseListen( TokenStream& tokensToParse )
{
	Node*	newNode;

	tokensToParse.moveToNextToken();
	newNode = new Node(N_LISTEN);
	if (!acceptAndCreateTerminal(tokensToParse, newNode))
		return (deleteNewNode(newNode));
	if (!accept(tokensToParse, T_SEMICOLON))
		return (deleteNewNode(newNode));
	return (newNode);
}

Node*	parseServerName( TokenStream& tokensToParse )
{
	Node*	newNode;

	tokensToParse.moveToNextToken();
	newNode = new Node(N_SERVER_NAME);
	if (!acceptAndCreateTerminal(tokensToParse, newNode))
		return (deleteNewNode(newNode));
	if (!accept(tokensToParse, T_SEMICOLON))
		return (deleteNewNode(newNode));
	return (newNode);
}

Node*	parseServer( TokenStream& tokensToParse )
{
	Node*	newNode;
	int		status = 1;

	tokensToParse.moveToNextToken();
	if (!accept(tokensToParse, T_BRACKET_OPEN))
		return (NULL);
	newNode = new Node(N_SERVER);
	while (!accept(tokensToParse, T_BRACKET_CLOSE) && status != 0)
	{
		switch (tokensToParse.getTokenType())
		{
			case T_SERVER_NAME:
				status = newNode->addChild(parseServerName(tokensToParse));
				break;
			case T_LISTEN:
				status = newNode->addChild(parseListen(tokensToParse));
				break;
			case T_ROOT:
				status = newNode->addChild(parseRoot(tokensToParse));
				break;
			case T_INDEX:
				status = newNode->addChild(parseIndex(tokensToParse));
				break;
			case T_CLIENT_MAX_BODY:
				status = newNode->addChild(parseClientMaxBodySize(tokensToParse));
				break;
			case T_LOCATION:
				status = newNode->addChild(parseLocation(tokensToParse));
				break;
			case T_ERROR_PAGE:
				status = newNode->addChild(parseErrorPage(tokensToParse));
				break;
			default:
				status = 0;
				break;
		}
	}
	if (status == 0)
		return (deleteNewNode(newNode));
	return (newNode);
}

Node*	parser( TokenStream& tokensToParse )
{
	Node*	ast;

	ast = new Node(AST);
	while (expect(tokensToParse, T_SERVER) && tokensToParse.isEmpty() == false)
	{
		if (ast->addChild(parseServer(tokensToParse)) == 0)
			return (deleteNewNode(ast));
	}
	return (ast);
}

