
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

void	Node::addNodeToList( Node* newNode )
{
	_nodes.push_back(newNode);
}