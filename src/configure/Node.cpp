
#include "Node.hpp"

/*
** ------------------------------- CONSTRUCTOR --------------------------------
*/

Node::Node()
{
}

Node::Node( const Node& src )
{
}

Node::Node( int nodeType, std::string terminal )
	: _nodeType(nodeType), _terminal(terminal)
{
}

/*
** -------------------------------- DESTRUCTOR --------------------------------
*/

Node::~Node()
{
}

/*
** --------------------------------- OVERLOAD ---------------------------------
*/

std::ostream&	operator<<( std::ostream& o, Node const& i )
{
	o << i.getValue();
	return o;
}

/*
** --------------------------------- METHODS ----------------------------------
*/

void	Node::addChild( Node* newNode )
{
	_nodes.push_back(newNode);
}