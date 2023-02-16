#ifndef TOKENSTREAM_HPP
# define TOKENSTREAM_HPP

# include <list>
# include "Token.hpp"

class TokenStream
{
	public:
		TokenStream( std::list<Token> tokenList );
		~TokenStream();

		std::string	getTokenString();
		int			getTokenType();
		bool		moveToNextToken();
		bool		isEmpty() const;
	private:
		std::list<Token>			_tokenList;
		std::list<Token>::iterator	_currentToken;
};

#endif
