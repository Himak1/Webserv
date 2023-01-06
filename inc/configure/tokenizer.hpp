
#ifndef TOKENIZER_HPP
# define TOKENIZER_HPP

# include <fstream>
# include <vector>
# include <map>
# include "Token.hpp"

typedef std::map<std::string, int> TokenMap;

std::vector<Token>	tokenizer( std::ifstream& file );
void				initMap( TokenMap& tokenTypes );
int					checkTokenType( const TokenMap& tokenTypes, const std::string& token );

#endif
