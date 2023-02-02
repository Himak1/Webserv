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
		bool			isEmpty() const;
	private:
		std::list<Token*>			_tokenList;
		std::list<Token*>::iterator	_currentToken;
};

#endif
