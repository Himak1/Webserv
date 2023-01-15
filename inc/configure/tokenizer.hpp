
#ifndef TOKENIZER_HPP
# define TOKENIZER_HPP

# include <fstream>
# include <list>
# include <map>
# include "Token.hpp"

typedef std::list<Token*>	TokenList;
typedef std::map<std::string, int> TokenMap;

std::list<Token*>	tokenizer( std::ifstream& file );
void				initMap( TokenMap& tokenTypes );
int					checkTokenType( const TokenMap& tokenMap, const std::string& token );

#endif
