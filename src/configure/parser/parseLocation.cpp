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

Node*   parseLocationAlias( TokenList::iterator& pos, const TokenList::iterator& ending )
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

Node*	parseLocationAllowedMethods( TokenList::iterator& pos, const TokenList::iterator& ending )
{
	//Node*	new
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
	newNode->addChild(parseLocationAlias(pos, ending));
	if (!accept(pos, T_BRACKET_CLOSE))
		return (deleteNewNode(newNode));
	// if (status == 0)
	// 	handle_parse_error();
	// if (token == bracket_open)
	// 	continue
	// while (token != bracket_close)
	// {
	// 	if (token == )

	// }
	return (newNode);
}
