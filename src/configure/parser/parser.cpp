
#include <list>
#include "parser.hpp"
#include "Node.hpp"
#include "Token.hpp"
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

Node*	parseServerName( TokenStream& tokensToParse )
{
	Node*	newNode;

	tokensToParse.moveToNextToken();
	if (!acceptAndCreateTerminal(tokensToParse, newNode))
		return (deleteNewNode(newNode));
	if (!accept(tokensToParse, T_SEMICOLON))
		return (deleteNewNode(newNode));
	return (newNode);
}

Node*	parseServer( TokenStream& tokensToParse )
{
	Node*	newNode;
	int		status;

	tokensToParse.moveToNextToken();
	if (!accept(tokensToParse, T_BRACKET_OPEN))
		return (deleteNewNode(newNode));
	while (!accept(tokensToParse, T_BRACKET_CLOSE))
	{
		switch (tokensToParse.getTokenType())
		{
			case T_SERVER_NAME:
				status = newNode->addChild(parseServerName(tokensToParse));
				break;
			case T_LISTEN:
				status = newNode->addChild(parseListen(tokensToParse));
				break;
			case T_LOCATION:
				status = newNode->addChild(parseLocation(tokensToParse));
				break;
			case T_ERROR_PAGE:
				status = newNode->addChild(parseErrorPage(tokensToParse));
				break;
		}
	}
	return (newNode);
}

/* Node*	parser( TokenStream& tokensToParse ) */
/* { */
/* 	Node*	ast; */

/* 	while (!accept(tokensToParse, T_SERVER)) */
/* 	{ */
		
/* 	} */
/* 	return (ast); */
/* } */
