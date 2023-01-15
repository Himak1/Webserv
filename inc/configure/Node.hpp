
#ifndef NODE_HPP
# define NODE_HPP

# include <iostream>
# include <string>
# include <list>

class Node
{
	public:
		Node( Node const& src );c
		Node( int nodeType, std::string terminal)
		~Node();

		void	addChild();
		// void accept( Visitor visitor );
	private:
		Node();

		int					_nodeType;
		std::string			_terminal;
		std::list<Node*>	_children;
};

std::ostream&	operator<<( std::ostream& o, Node const& i );

#endif
