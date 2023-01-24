
#ifndef NODE_HPP
# define NODE_HPP

# include <iostream>
# include <string>
# include <list>

class Node
{
	public:
		Node( int nodeType, std::string terminal);
		~Node();

		int		getNodeType() const;
		int		addChild( Node* newNode );
	private:
		Node();

		int					_nodeType;
		std::string			_terminal;
		std::list<Node*>	_children;
};

std::ostream&	operator<<( std::ostream& o, Node const& i );

#endif
