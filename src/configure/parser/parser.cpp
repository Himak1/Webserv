
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

Node*	parseLocation( TokenList::iterator& pos, const TokenList::iterator& end )
{
	Node*	newNode;

	// if ((*pos)->GetTokenType() != LOCATION))
	// 	return (0);
	// pos++;
	// if ((*pos)->GetTokenType() != BRACKET_OPEN)
	// 	return (0);
	// pos++;
	// if ((*pos)->GetTokenType() != BRACKET_CLOSE)
	// 	return (0);
	// pos++;

	/*
		if (token == location)
			continue
		if (token == string)
			continue
		if (token == bracket_open)
			continue
		while (token != bracket_close)
		{
			if (token == )
		}
	*/
	return (new Node())
}

/*

*/