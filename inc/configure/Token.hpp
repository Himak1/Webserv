#ifndef TOKEN_HPP
# define TOKEN_HPP

# include <iostream>
# include <string>

enum TokenTypes
{
	INVALID_TOKEN = 0,
	SERVER_NAME,
	LISTEN,
	PORT,
	ACCESS_LOG,
	LOCATION,
	ROOT,
	EXPIRES,
	STRING,
	ERROR_PAGE,
	// Punctuation and Seperators
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
		Token( int type, std::string tokenString );
		Token( Token const& src );
		~Token();

		Token&		operator=( Token const& rhs );

		int				getTokenType() const;
		const std::string&	getToken() const;
	private:
		int			_tokenType;
		std::string	_token;
};

std::ostream&		operator<<( std::ostream& o, Token const& i );

#endif
