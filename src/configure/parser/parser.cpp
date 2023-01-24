
#include <list>
#include "parser.hpp"
#include "Node.hpp"

// Node*	parser( TokenList tList )
// {
// 	TokenList::iterator			pos = tokenList.begin();
// 	const TokenList::iterator	end = tokenList.end();
// 	Node*						ast = new Node();

// 	while (pos != end && status != -1)
// 	{
// 		ast.addNewNode();
// 		ast._nodes() = server(pos, end);
// 		iter++;
// 	}
// 	return (status);
// }

// NodeList	server( TokenList::iterator& pos, const TokenList::iterator& end );
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

bool	accept( TokenList::iterator& pos, int expected_token )
{
	if ((*pos)->getTokenType() == expected_token)
	{
		pos++;
		return (true);
	}
	return (false);
}
