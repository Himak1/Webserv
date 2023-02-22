
#ifndef TOKEN_HPP
# define TOKEN_HPP

# include <iostream>
# include <string>
# include <map>

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
	T_UPLOAD_STORE,
	// Punctuation and Seperators
	T_BRACKET_OPEN,
	T_BRACKET_CLOSE,
	T_SEMICOLON,
	T_HASHTAG,
	T_DOLLAR,
	T_HOME_DIR
};

typedef std::map<std::string, int> TokenMap;

class Token
{
	public:
		Token();
		Token( int type, std::string tokenString, int lineNumber );
		Token( Token const& src );
		~Token();

		Token&		operator=( Token const& rhs );

		int			getTokenType() const;
		std::string	getToken() const;
		int			getLineNumber() const;
	private:
		int			_tokenType;
		std::string	_token;
		int			_lineNumber;
};

std::ostream&		operator<<( std::ostream& o, Token const& i );

#endif
