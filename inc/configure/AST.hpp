
#ifndef AST_HPP
# define AST_HPP

# include <iostream>
# include <string>
# include <list>

typedef	std::list<AST*>	Tree;

class AST
{
	public:
		AST();
		AST( AST const& src );
		~AST();

		AST&		operator=( AST const& rhs );
	private:
		int			_nodeType;
		std::string	_value;
		Tree		node;
};

std::ostream&		operator<<( std::ostream& o, AST const& i );

#endif
