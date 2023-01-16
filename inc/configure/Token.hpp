
#ifndef TOKEN_HPP
# define TOKEN_HPP

# include <iostream>
# include <string>

enum TokenTypes
{
	STRING = 0,
	SERVER,
	SERVER_NAME,
	LISTEN,
	PORT,
	ACCESS_LOG,
	LOCATION,
	ROOT,
	EXPIRES,
	ERROR_PAGE,
	RETURN,
	// Punctuation and Seperators
	BRACKET_OPEN,
	BRACKET_CLOSE,
	FORWARD_SLASH,
	BACK_SLASH,
	SEMICOLON,
	HASHTAG,
	DOLLAR,
	HOME_DIR
};

class Token
{
	public:
		Token( int type, std::string tokenString );
		Token( Token const& src );
		~Token();

		Token&		operator=( Token const& rhs );

		int			getTokenType() const;
		const std::string&	getToken() const;
	private:
		Token();
		int			_tokenType;
		std::string	_token;
};

std::ostream&		operator<<( std::ostream& o, Token const& i );

#endif