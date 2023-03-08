
#ifndef NODE_HPP
# define NODE_HPP

# include <iostream>
# include <string>
# include <list>

using namespace std;

enum NodeTypes
{
	N_SERVER = 100,
	N_SERVER_NAME,
	N_LISTEN,
	N_INDEX,
	N_CLIENT_MAX_BODY,
	N_LOCATION,
	N_ROOT,
	N_EXPIRES,
	N_ERROR_PAGE,
	N_RETURN,
	N_ALLOWED_METHODS,
	N_CGI_PASS,
	N_ALIAS,
	N_AUTOINDEX,
	N_UPLOAD_STORE,
	TERMINAL,
	AST
};

class Node;
typedef list<Node*> NodeList;

class Node
{
	public:
		Node( int nodeType );
		Node( int nodeType, string terminal);
		~Node();

		int							getNodeType() const;
		string						getTerminal() const;
		NodeList::const_iterator	getChildrenBegin() const;
		NodeList::const_iterator	getChildrenEnd() const;
		int							addChild( Node* newNode );
	private:
		Node();

		int					_nodeType;
		string			_terminal;
		NodeList			_children;
};

ostream&	operator<<( ostream& o, Node const& i );

#endif
