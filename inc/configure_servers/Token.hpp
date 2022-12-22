#ifndef TOKEN_HPP
# define TOKEN_HPP

# include <iostream>
# include <string>

class Token
{

	public:

		Token();
		Token( Token const & src );
		~Token();

		Token &		operator=( Token const & rhs );

	private:

};

std::ostream &			operator<<( std::ostream & o, Token const & i );

#endif /* *********************************************************** TOKEN_H */