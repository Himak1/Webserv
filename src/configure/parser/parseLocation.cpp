#include <list>
#include "parser.hpp"
#include "Node.hpp"

bool	checkTokenSequence(std::vector<int>& sequence, TokenList::iterator pos, const TokenList::iterator& ending)
{
	std::vector<int>::iterator	seqIter = sequence.begin();

	while (seqIter != sequence.end() && pos != ending)
	{
		if (*seqIter == (*pos)->getTokenType)
		++seqIter;
		++pos;
	}
	return (0);
}

Node*   parseLocationAlias( TokenList::iterator& pos, const TokenList::iterator& ending )
{
	Node*	newNode;
	std::vector<int>	tokenSequence = {T_STRING, T_SEMICOLON};

	if (!accept(pos, T_ALIAS))
		return (NULL);
	newNode = new Node(N_ALIAS);
	if (checkTokenSequence(tokenSequence))
		newNode.addChild(new Node(TERMINAL, (*pos)->getToken()));
	++pos;
	++pos;
}

Node*	parseLocationPath( TokenList::iterator& pos, const TokenList::iterator& ending )
{
	Node*	newNode;

	if (pos == ending)
		return (NULL);
	if ((*pos)->getTokenType() != T_STRING)
		return (NULL);
	newNode = new Node(STRING, (*pos)->getToken());
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