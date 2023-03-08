#ifndef TOKENIZER_HPP
# define TOKENIZER_HPP

# include <fstream>
# include <list>
# include <map>

using namespace std;

typedef map<string, int> TokenMap;

class Token;
class TokenStream;

TokenStream*	tokenizer( ifstream& file );
list<Token>		createTokenList( ifstream& file, TokenMap tokenMap );
list<string>	splitLineByDelimiters( string line );
list<Token>		createTokensFromStrings( list<string> splitStrings, TokenMap tokenMap );

void			initMap( TokenMap& tokenTypes );
int				checkTokenType( const TokenMap& tokenMap, const string& token );

#endif
