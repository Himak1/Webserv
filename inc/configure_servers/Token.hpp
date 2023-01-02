#ifndef TOKEN_HPP
# define TOKEN_HPP

# include <iostream>
# include <string>

enum TokenTypes
{
	INVALID_TOKEN = 0,
	BRACKET_OPEN,
	BRACKET_CLOSE,
	FORWARD_SLASH,
	BACKWARDS_SLASH,
	SEMICOLON,
	HASHTAG,
	DOLLAR
};

class Token
{
	public:
		Token();
		Token( Token const& src );
		~Token();

		Token&		operator=( Token const& rhs );

		int&			getToken() const;
		std::string&	getTokenType() const;
	private:
		int			_tokenType;
		std::string	_token;
};

std::ostream&		operator<<( std::ostream& o, Token const& i );

#endif
