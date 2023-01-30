#include <list>
#include <vector>
#include "parser.hpp"
#include "Token.hpp"
#include "Node.hpp"

bool	checkTokenSequence(int* sequence, int seqLength, TokenList::iterator pos, const TokenList::iterator& ending)
{
	int	i = 0;

	while (i < seqLength && pos != ending)
	{
		if (sequence[i] != (*pos)->getTokenType())
			return (0);
		i++;
		++pos;
	}
	return (1);
}

Node*   parseAlias( TokenList::iterator& pos, const TokenList::iterator& ending )
{
	Node*	newNode;
	int		tokenSequence[] = {T_STRING, T_SEMICOLON};

	if (!accept(pos, T_ALIAS))
		return (NULL);
	newNode = new Node(N_ALIAS);
	if (checkTokenSequence(tokenSequence, sizeof(tokenSequence), pos, ending))
		newNode->addChild(new Node(TERMINAL, (*pos)->getToken()));
	++pos;
	++pos;
	return (newNode);
}

Node*	parseAllowedMethods( TokenList::iterator& pos, const TokenList::iterator& ending )
{
	Node*	newNode;

	accept(pos, T_ALLOWED_METHODS);
	newNode = new Node(N_ALLOWED_METHODS);
	while (pos != ending && (*pos)->getTokenType() == T_STRING)
	{
		newNode->addChild(new Node(TERMINAL, (*pos)->getToken()));
		++pos;
	}
	if (pos != ending && !accept(pos, T_SEMICOLON))
		return (deleteNewNode(newNode));
	if (pos != ending && !accept(pos, T_BRACKET_CLOSE))
		return (deleteNewNode(newNode));
	return (newNode);
}

Node*	parseCgiPass( TokenList::iterator& pos, const TokenList::iterator& ending )
{
	Node*	newNode;

	accept(pos, T_CGI_PASS);
	if (!accept(pos, T_STRING))
		return (deleteNewNode(newNode));
	if (!accept(pos, T_STRING))
		return (deleteNewNode(newNode));
	if (!accept(pos, T_SEMICOLON))
		return (deleteNewNode(newNode));
}

Node*	parseLocationPath( TokenList::iterator& pos, const TokenList::iterator& ending )
{
	Node*	newNode;

	if (pos == ending)
		return (NULL);
	if ((*pos)->getTokenType() != T_STRING)
		return (NULL);
	newNode = new Node(T_STRING, (*pos)->getToken());
	return (newNode);
}

Node*	parseLocation( TokenList::iterator& pos, const TokenList::iterator& ending )
{
	Node*	newNode;
	int		status;

	if (!accept(pos, T_LOCATION))
		return (NULL);
	newNode = new Node(123);
	status = newNode->addChild(parseLocationPath(pos, ending));
	if (status == 0)
		return (deleteNewNode(newNode));
	if (!accept(pos, T_BRACKET_OPEN))
		return (deleteNewNode(newNode));
	while (pos != ending && !accept(pos, T_BRACKET_CLOSE) && status != 0)
	{
		switch ((*pos)->getTokenType())
		{
			case T_ALIAS:
				status = newNode->addChild(parseAlias(pos, ending));
				break;
			case T_ALLOWED_METHODS:
				status = newNode->addChild(parseAllowedMethods(pos, ending));
				break;
			case T_CGI_PASS:
				status = newNode->addChild(parseCgiPass(pos, ending));
				break;
			default:
				status = 0;
				break;
		}
	}
	return (newNode);
}

