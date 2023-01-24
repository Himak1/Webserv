
#ifndef TOKENIZER_HPP
# define TOKENIZER_HPP

# include <fstream>
# include <list>
# include <map>
# include "Token.hpp"

typedef std::list<Token*>	TokenList;
typedef std::map<std::string, int> TokenMap;

std::list<Token*>		tokenizer( std::ifstream& file );
std::list<std::string>	splitLine( std::string line );
void					initMap( TokenMap& tokenTypes );
int						checkTokenType( const TokenMap& tokenMap, const std::string& token );

#endif
