
#include <string>
#include <map>
#include "tokenizer.hpp"
#include "Token.hpp"

int	checkTokenType( const TokenMap& tokenTypes, const std::string& token )
{
	TokenMap::const_iterator	it;

	it = tokenTypes.find(token);
	if (it == tokenTypes.end())
		return (STRING);
	return (it->second);
}
