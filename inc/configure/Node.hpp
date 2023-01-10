
#ifndef NODE_HPP
# define NODE_HPP

# include <iostream>
# include <string>
# include <list>

class Node
{
	public:
		Node();
		Node( Node const& src );
		~Node();

		void	addNodeToList();
		// void accept( Visitor visitor );
	private:
		int						_nodeType;
		std::list<std::string>	_terminals;
		std::list<Node*>		_nodes;
};

std::ostream&	operator<<( std::ostream& o, Node const& i );

#endif
