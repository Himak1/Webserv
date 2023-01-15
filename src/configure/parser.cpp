
#include <list>
#include "parser.hpp"

NodeList	parse( TokenList tList )
{
	std::list<Token*>::iterator	iter;
	Node*						ast;

	iter = tokenList.begin();
	ast = new Node();
	while (iter != tokenList.end() && status != -1)
	{
		ast.addNewNode();
		ast._nodes() = server(iter);
		iter++;
	}
	return (status);
}

NodeList	server( TokenList tList)
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

Node*	parseLocation( TokenList tList, TokenList::iterator& pos )
{
	Node*	newNode;

	if ((*pos)->GetTokenType() != LOCATION))
		return (0);
	pos++;
	if ((*pos)->GetTokenType() != BRACKET_OPEN)
		return (0);
	pos++;
	if ((*pos)->GetTokenType() != BRACKET_CLOSE)
		return (0);
	pos++;
	return (new Node())
}

/*

*/