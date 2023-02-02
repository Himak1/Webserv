#include <list>
#include <vector>
#include "parser.hpp"
#include "Token.hpp"
#include "TokenStream.hpp"
#include "Node.hpp"

/* bool	checkTokenSequence(int* sequence, int seqLength, TokenList::iterator currentToken, const TokenList::iterator& ending) */
/* { */
/* 	int	i = 0; */

/* 	while (i < seqLength && currentToken != ending) */
/* 	{ */
/* 		if (sequence[i] != (*currentToken)->getTokenType()) */
/* 			return (0); */
/* 		i++; */
/* 		++currentToken; */
/* 	} */
/* 	return (1); */
/* } */

/* Node*   parseAlias( TokenList::iterator& currentToken, const TokenList::iterator& ending ) */
/* { */
/* 	Node*	newNode; */
/* 	int		tokenSequence[] = {T_STRING, T_SEMICOLON}; */

/* 	if (!accept(currentToken, ending, T_ALIAS)) */
/* 		return (NULL); */
/* 	newNode = new Node(N_ALIAS); */
/* 	if (checkTokenSequence(tokenSequence, sizeof(tokenSequence), currentToken, ending)) */
/* 		newNode->addChild(new Node(TERMINAL, (*currentToken)->getToken())); */
/* 	++currentToken; */
/* 	++currentToken; */
/* 	return (newNode); */
/* } */

/* Node*	parseRoot( TokenList::iterator& currentToken, const TokenList::iterator& ending ) */
/* { */
/* 	Node*	newNode; */

/* 	accept(currentToken, ending, T_ROOT); */
/* 	newNode = new Node(N_ROOT); */
/* 	if (!(accept(currentToken, ending, T_STRING) */
/* 		&& accept(currentToken, ending, T_SEMICOLON))) */
/* 		return (deleteNewNode(newNode)); */
/* 	return (newNode); */
/* } */

Node*	parseAllowedMethods( TokenStream& tokensToParse )
{
	Node*	newNode;

	accept(tokensToParse, T_ALLOWED_METHODS);
	newNode = new Node(N_CGI_PASS);
	for (int i = 0; i < HTTP_METHODS; i++)
	{
		if (expect(tokensToParse, T_STRING))
		{
			newNode->addChild(new Node(TERMINAL, tokensToParse.getTokenString()));
			tokensToParse.moveToNextToken();
		}
		else
			break;
	}
	if (!accept(tokensToParse, T_SEMICOLON))
		return (deleteNewNode(newNode));
	return (newNode);
}

/* Node*	parseCgiPass( TokenList::iterator& currentToken, const TokenList::iterator& ending ) */
/* { */
/* 	Node*	newNode; */

/* 	accept(currentToken, ending, T_CGI_PASS); */
/* 	newNode = new Node(N_CGI_PASS); */
/* 	if (!accept(currentToken, ending, T_STRING) */
/* 		|| !accept(currentToken, ending, T_STRING) */
/* 		|| !accept(currentToken, ending, T_SEMICOLON)) */
/* 		return (deleteNewNode(newNode)); */
/* 	return (newNode); */
/* } */

/* Node*	parseLocationPath( TokenList::iterator& currentToken, const TokenList::iterator& ending ) */
/* { */
/* 	Node*	newNode; */

/* 	if (currentToken == ending) */
/* 		return (NULL); */
/* 	if ((*currentToken)->getTokenType() != T_STRING) */
/* 		return (NULL); */
/* 	newNode = new Node(T_STRING, (*currentToken)->getToken()); */
/* 	++currentToken; */
/* 	return (newNode); */
/* } */

/* Node*	parseLocation( TokenList::iterator& currentToken, const TokenList::iterator& ending ) */
/* { */
/* 	Node*	newNode; */
/* 	int		status; */

/* 	if (!accept(currentToken, ending, T_LOCATION)) */
/* 		return (NULL); */
/* 	newNode = new Node(N_LOCATION); */
/* 	status = newNode->addChild(parseLocationPath(currentToken, ending)); */
/* 	if (status == 0) */
/* 		return (deleteNewNode(newNode)); */
/* 	if (!accept(currentToken, ending, T_BRACKET_OPEN)) */
/* 		return (deleteNewNode(newNode)); */
/* 	while (!accept(currentToken, ending, T_BRACKET_CLOSE) && status != 0) */
/* 	{ */
/* 		switch ((*currentToken)->getTokenType()) */
/* 		{ */
/* 			case T_ALIAS: */
/* 				status = newNode->addChild(parseAlias(currentToken, ending)); */
/* 				break; */
/* 			case T_ROOT: */
/* 				status = newNode->addChild(parseRoot(currentToken, ending)); */
/* 				break; */
/* 			case T_ALLOWED_METHODS: */
/* 				status = newNode->addChild(parseAllowedMethods(currentToken, ending)); */
/* 				break; */
/* 			case T_CGI_PASS: */
/* 				status = newNode->addChild(parseCgiPass(currentToken, ending)); */
/* 				break; */
/* 			default: */
/* 				status = 0; */
/* 				break; */
/* 		} */
/* 	} */
/* 	return (newNode); */
/* } */

