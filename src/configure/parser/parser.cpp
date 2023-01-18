
#include <list>
#include "parser.hpp"

Node*	parser( TokenList tList )
{
	TokenList::iterator			pos = tokenList.begin();
	const TokenList::iterator	end = tokenList.end();
	Node*						ast = new Node();

	while (pos != end && status != -1)
	{
		ast.addNewNode();
		ast._nodes() = server(pos, end);
		iter++;
	}
	return (status);
}

NodeList	server( TokenList::iterator& pos, const TokenList::iterator& end );
{
	while ()
	{
		switch (iter->getTokenType())
		{
			case LOCATION:
				status = parseLocation();
				break ;
			case PORT:
				status = parsePort();
				break ;
			case SERVER_NAME:
				status = parseServerName();
				break ;
			default:
				status = -1;
				break ;
		}
	}
}

Node*	parsePort()
{

}

Node*	parseLocationPath( TokenList::iterator& pos, const TokenList::iterator& end )
{
	Node*	newNode;

}

Node*	parseLocation( TokenList::iterator& pos, const TokenList::iterator& end )
{
	Node*	newNode;

	if (!accept(pos, LOCATION))
		return (NULL);
	newNode = new Node();
	status = newNode.addChild(parseLocationPath(pos, end));
	if (status == 0)
		handle_parse_error();
	if (token == bracket_open)
		continue
	while (token != bracket_close)
	{
		if (token == )

	}
	return (new Node())
	*/
}

/*
bool	accept( TokenList::iterator& pos, int expected_token )
{
	if ((*pos)->getTokenType() == expected_token)
	{
		pos++;
		return (true);
	}
	return (false);
}

bool	expect( TokenList::iterator& pos, int expected_token )
{
	if ((*pos)->getTokenType() == expected_token)
	{
		return (true);
	}
	return (false);
}

*/
