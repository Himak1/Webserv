#include <list>
#include "parser.hpp"
#include "Node.hpp"

Node*	parseLocationPath( TokenList::iterator& pos, const TokenList::iterator& end )
{
	Node*	newNode;

	if (pos == end)
		return (NULL);
	if ((*pos)->getTokenType() != STRING)
		return (NULL);
	newNode = new Node(STRING, (*pos)->getToken());
	return (newNode);
}

Node*	parseLocation( TokenList::iterator& pos, const TokenList::iterator& end )
{
	Node*	newNode;
	int		status;

	if (!accept(pos, LOCATION))
		return (NULL);
	newNode = new Node(123, "");
	status = newNode->addChild(parseLocationPath(pos, end));
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