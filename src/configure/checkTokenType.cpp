
#include <string>
#include <map>
#include "tokenizer.hpp"
#include "Token.hpp"

int	checkTokenType( const std::map<std::string, int>& tokenTypes, const std::string& token )
{
	std::map<std::string, int>::const_iterator	it;

	it = tokenTypes.find(token);
	if (it == tokenTypes.end())
		return (STRING);
	return (it->second);
}
