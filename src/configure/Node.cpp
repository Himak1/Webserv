
#include "Node.hpp"

/*
** ------------------------------- CONSTRUCTOR --------------------------------
*/

Node::Node()
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
	o << i.getNodeType();
	return o;
}

/*
** --------------------------------- METHODS ----------------------------------
*/

int		Node::getNodeType() const
{
	return (_nodeType);
}

int		Node::addChild( Node* newNode )
{
	_children.push_back(newNode);
	return (0);
}