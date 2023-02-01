#ifndef TOKENSTREAM_HPP
# define TOKENSTREAM_HPP

# include <list>

class Token;

class TokenStream
{
	public:
		TokenStream( std::list<Token*> tokenList );
		~TokenStream();

		const Token*	getToken() const;
		bool			moveToNextToken();
	private:
		std::list<Token*>			_tokenList;
		std::list<Token*>::iterator	_currentToken;
};

#endif
