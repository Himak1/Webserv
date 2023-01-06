
#ifndef TOKENIZER_HPP
# define TOKENIZER_HPP

# include <fstream>
# include <vector>
# include <map>
# include "Token.hpp"

std::vector<Token>	tokenizer( std::ifstream& file );
void				initMap( std::map<std::string, int>& tokenTypes );
int					checkTokenType( const std::map<std::string, int>& tokenTypes, const std::string& token );

#endif
