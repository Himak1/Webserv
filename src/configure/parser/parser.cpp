#include <list>
#include "parser.hpp"
#include "../Node.hpp"
#include "Token.hpp"
#include "TokenStream.hpp"

Node*	parseUploadStore( TokenStream& tokensToParse )
{
	Node*	newNode;

	tokensToParse.moveToNextToken();
	newNode = new Node(N_UPLOAD_STORE);
	if (!acceptAndCreateTerminal(tokensToParse, newNode))
		return (deleteNewNode(newNode));
	if (!accept(tokensToParse, T_SEMICOLON))
		return (deleteNewNode(newNode));
	return (newNode);
}

Node*	parseReturn( TokenStream& tokensToParse )
{
	Node*	newNode;

	tokensToParse.moveToNextToken();
	newNode = new Node(N_RETURN);
	if (!acceptAndCreateTerminal(tokensToParse, newNode))
		return (deleteNewNode(newNode));	
	if (!acceptAndCreateTerminal(tokensToParse, newNode))
		return (deleteNewNode(newNode));
	if (!accept(tokensToParse, T_SEMICOLON))
		return (deleteNewNode(newNode));
	return (newNode);
}

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
	while (!tokensToParse.isEmpty() && tokensToParse.getTokenType() == T_STRING)
		acceptAndCreateTerminal(tokensToParse, newNode);
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
	while (blockIsOpen(tokensToParse, status))
	{
		switch (tokensToParse.getTokenType()) {
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
			case T_RETURN:
				status = newNode->addChild(parseReturn(tokensToParse));
				break;
			case T_UPLOAD_STORE:
				status = newNode->addChild(parseUploadStore(tokensToParse));
				break;
			default:
				printUnexpectedChar(tokensToParse);
				status = 0;
				break;
		}
	}
	if (status == 0)
		return (deleteNewNode(newNode));
	tokensToParse.moveToNextToken();
	return (newNode);
}

Node*	parser( TokenStream& tokensToParse )
{
	Node*	ast;

	ast = new Node(AST);
	while (tokensToParse.isEmpty() == false) {
		if (!expect(tokensToParse, T_SERVER))
			return (deleteNewNode(ast));
		if (ast->addChild(parseServer(tokensToParse)) == 0)
			return (deleteNewNode(ast));
	}
	return (ast);
}

