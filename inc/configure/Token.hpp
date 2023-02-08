
#ifndef TOKEN_HPP
# define TOKEN_HPP

# include <iostream>
# include <string>

enum TokenTypes
{
	T_STRING = 0,
	T_SERVER,
	T_SERVER_NAME,
	T_LISTEN,
	T_INDEX,
	T_CLIENT_MAX_BODY,
	T_PORT,
	T_ACCESS_LOG,
	T_LOCATION,
	T_ROOT,
	T_EXPIRES,
	T_ERROR_PAGE,
	T_RETURN,
	T_ALLOWED_METHODS,
	T_CGI_PASS,
	T_ALIAS,
	T_AUTOINDEX,
	// Punctuation and Seperators
	T_BRACKET_OPEN,
	T_BRACKET_CLOSE,
	T_FORWARD_SLASH,
	T_BACK_SLASH,
	T_SEMICOLON,
	T_HASHTAG,
	T_DOLLAR,
	T_HOME_DIR
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
