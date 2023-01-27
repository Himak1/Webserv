
#ifndef NODE_HPP
# define NODE_HPP

# include <iostream>
# include <string>
# include <list>

enum NodeTypes
{
	N_SERVER = 100,
	N_SERVER_NAME,
	N_LISTEN,
	N_PORT,
	N_LOCATION,
	N_ROOT,
	N_EXPIRES,
	N_ERROR_PAGE,
	N_RETURN,
	N_ALLOWED_METHODS,
	N_CGI_PASS,
	N_ALIAS,
	TERMINAL
};

class Node
{
	public:
		Node( int nodeType );
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
