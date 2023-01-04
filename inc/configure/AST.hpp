#ifndef AST_HPP
# define AST_HPP

# include <iostream>
# include <string>

class AST
{

	public:

		AST();
		AST( AST const & src );
		~AST();

		AST &		operator=( AST const & rhs );

	private:

};

std::ostream &			operator<<( std::ostream & o, AST const & i );

#endif /* ************************************************************* AST_H */