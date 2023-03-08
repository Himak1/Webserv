#ifndef TOKENSTREAM_HPP
# define TOKENSTREAM_HPP

# include <list>
# include "Token.hpp"

using namespace std;

class TokenStream
{
	public:
		TokenStream( list<Token> tokenList );
		~TokenStream();

		string					getTokenString();
		int						getTokenType();
		int						getCurrentLine();
		bool					moveToNextToken();
		bool					isEmpty() const;

	private:
		list<Token>				_tokenList;
		list<Token>::iterator	_currentToken;
		TokenMap				_tokenMap;
};

#endif
