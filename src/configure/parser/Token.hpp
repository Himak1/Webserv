
#ifndef TOKEN_HPP
# define TOKEN_HPP

# include <iostream>
# include <string>
# include <map>

using namespace std;

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

typedef map<string, int> TokenMap;

class Token
{
	public:
		Token();
		Token( int type, string tokenString, int lineNumber );
		Token( Token const& src );
		~Token();

		Token&		operator=( Token const& rhs );

		int			getTokenType() const;
		string		getToken() const;
		int			getLineNumber() const;
	private:
		int			_tokenType;
		string		_token;
		int			_lineNumber;
};

ostream&		operator<<( ostream& o, Token const& i );

#endif
