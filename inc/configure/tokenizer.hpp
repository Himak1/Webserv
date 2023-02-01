#ifndef TOKENIZER_HPP
# define TOKENIZER_HPP

# include <fstream>
# include <list>
# include <map>

typedef std::map<std::string, int> TokenMap;

class Token;
class TokenStream;

TokenStream				tokenizer( std::ifstream& file );
std::list<Token*>		createTokenList( std::ifstream& file, TokenMap tokenMap );
std::list<std::string>	splitLineByDelimiters( std::string line );
std::list<Token*>		createTokensFromStrings( std::list<std::string> splitStrings, TokenMap tokenMap );

void					initMap( TokenMap& tokenTypes );
int						checkTokenType( const TokenMap& tokenMap, const std::string& token );

#endif
