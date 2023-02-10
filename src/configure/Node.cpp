
#include "Node.hpp"

/*
** ------------------------------- CONSTRUCTOR --------------------------------
*/

Node::Node()
{
}

Node::Node( int nodeType )
	: _nodeType(nodeType)
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
	std::list<Node*>::iterator	it;
	for (it = _children.begin(); it != _children.end(); ++it)
	{
		delete *it;
	}
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

const int&	Node::getNodeType() const
{
	return (_nodeType);
}

const std::string&	Node::getTerminal() const
{
	return (_terminal);
}

NodeList::const_iterator	Node::getChildrenBegin() const
{
	return (_children.begin());
}

NodeList::const_iterator	Node::getChildrenEnd() const
{
	return (_children.end());
}

int		Node::addChild( Node* newNode )
{
	if (newNode == NULL)
		return (0);
	_children.push_back(newNode);
	return (1);
}
