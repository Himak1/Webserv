
#include <list>
#include "parser.hpp"

Node*	parser( TokenList tokenList )
{
	std::list<Token*>::iterator	iter;
	Node*						ast;

	iter = tokenList.begin();
	ast = new Node();
	while (iter != tokenList.end() && status != -1)
	{
		ast.addNewNode()
		ast._nodes() = server(iter);
		iter++;
	}
	return (status);
}

Node*	server()
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

int	parsePort()
{

}

int	parseLocation()
{
	while ()
}
