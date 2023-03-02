#include <list>
#include <vector>
#include "parser.hpp"
#include "Token.hpp"
#include "TokenStream.hpp"
#include "../Node.hpp"

Node*	parseAlias( TokenStream& tokensToParse )
{
	Node*	newNode;

	accept(tokensToParse, T_ALIAS);
	newNode = new Node(N_ALIAS);
	if (expect(tokensToParse, T_STRING))
		acceptAndCreateNewNode(tokensToParse, newNode);
	else
		return (deleteNewNode(newNode));
	if (!accept(tokensToParse, T_SEMICOLON))
		return (deleteNewNode(newNode));
	return (newNode);
}

Node*	parseRoot( TokenStream& tokensToParse )
{
	Node*	newNode;

	accept(tokensToParse, T_ROOT);
	newNode = new Node(N_ROOT);
	if (!acceptAndCreateTerminal(tokensToParse, newNode))
		return (deleteNewNode(newNode));
	if (!accept(tokensToParse, T_SEMICOLON))
		return (deleteNewNode(newNode));
	return (newNode);
}

Node*	parseAllowedMethods( TokenStream& tokensToParse )
{
	Node*	newNode;

	accept(tokensToParse, T_ALLOWED_METHODS);
	newNode = new Node(N_ALLOWED_METHODS);
	for (int i = 0; i < HTTP_METHODS; i++)
	{
		if (!tokensToParse.isEmpty() && tokensToParse.getTokenType() == T_SEMICOLON)
			break;
		else if (expect(tokensToParse, T_STRING))
			acceptAndCreateNewNode(tokensToParse, newNode);
		else
			return (deleteNewNode(newNode));
	}
	if (!accept(tokensToParse, T_SEMICOLON))
		return (deleteNewNode(newNode));
	return (newNode);
}

Node*	parseCgiPass( TokenStream& tokensToParse )
{
	Node*	newNode;

	accept(tokensToParse, T_CGI_PASS);
	newNode = new Node(N_CGI_PASS);
	if (expect(tokensToParse, T_STRING))
		acceptAndCreateNewNode(tokensToParse, newNode);
	else
		return (deleteNewNode(newNode));
	if (expect(tokensToParse, T_STRING))
		acceptAndCreateNewNode(tokensToParse, newNode);
	else
		return (deleteNewNode(newNode));
	if (!accept(tokensToParse, T_SEMICOLON))
		return (deleteNewNode(newNode));
	return (newNode);
}

Node*	parseAutoIndex( TokenStream& tokensToParse )
{
	Node*	newNode;

	accept(tokensToParse, T_AUTOINDEX);
	newNode = new Node(N_AUTOINDEX);
	if (!acceptAndCreateTerminal(tokensToParse, newNode))
		return (deleteNewNode(newNode));
	if (!accept(tokensToParse, T_SEMICOLON))
		return (deleteNewNode(newNode));
	return (newNode);
}

Node*	parseLocationPath( TokenStream& tokensToParse )
{
	if (expect(tokensToParse, T_STRING))
	{
		Node* newNode = new Node(TERMINAL, tokensToParse.getTokenString());
		tokensToParse.moveToNextToken();
		return (newNode);
	}
	else
		return (NULL);
}

Node*	parseLocation( TokenStream& tokensToParse )
{
	Node*	newNode;
	int		status = 1;

	if (!accept(tokensToParse, T_LOCATION))
		return (NULL);
	newNode = new Node(N_LOCATION);
	if (!acceptAndCreateTerminal(tokensToParse, newNode))
		return (deleteNewNode(newNode));
	if (!accept(tokensToParse, T_BRACKET_OPEN))
		return (deleteNewNode(newNode));
	while (blockIsOpen(tokensToParse, status))
	{
		switch (tokensToParse.getTokenType())
		{
			case T_INDEX:
				status = newNode->addChild(parseIndex(tokensToParse));
				break;
			case T_RETURN:
				status = newNode->addChild(parseReturn(tokensToParse));
				break;
			case T_ALIAS:
				status = newNode->addChild(parseAlias(tokensToParse));
				break;
			case T_ROOT:
				status = newNode->addChild(parseRoot(tokensToParse));
				break;
			case T_ALLOWED_METHODS:
				status = newNode->addChild(parseAllowedMethods(tokensToParse));
				break;
			case T_CGI_PASS:
				status = newNode->addChild(parseCgiPass(tokensToParse));
				break;
			case T_AUTOINDEX:
				status = newNode->addChild(parseAutoIndex(tokensToParse));
				break;
			case T_ERROR_PAGE:
				status = newNode->addChild(parseErrorPage(tokensToParse));
				break;
			case T_UPLOAD_STORE:
				status = newNode->addChild(parseUploadStore(tokensToParse));
				break;
			case T_CLIENT_MAX_BODY:
				status = newNode->addChild(parseClientMaxBodySize(tokensToParse));
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
