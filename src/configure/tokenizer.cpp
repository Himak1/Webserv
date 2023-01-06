
#include <iostream>
#include <sstream>
#include <string>
#include <map>
#include "tokenizer.hpp"
#include "Token.hpp"

void	initMap( std::map<std::string, int>& tokenTypes )
{
	tokenTypes["server"]		= SERVER;
	tokenTypes["server_name"]	= SERVER_NAME;
	tokenTypes["listen"] 		= LISTEN;
	tokenTypes["port"] 			= PORT;
	tokenTypes["access_log"]	= ACCESS_LOG;
	tokenTypes["location"]		= LOCATION;
	tokenTypes["root"]			= ROOT;
	tokenTypes["expires"] 		= EXPIRES;
	tokenTypes["error_page"]	= ERROR_PAGE;
	tokenTypes["{"]				= BRACKET_OPEN;
	tokenTypes["}"]				= BRACKET_CLOSE;
	tokenTypes["/"]				= FORWARD_SLASH;
	tokenTypes["\\"]			= BACK_SLASH;
	tokenTypes[";"]				= SEMICOLON;
	tokenTypes["#"]				= HASHTAG;
	tokenTypes["$"]				= DOLLAR;
}

int	checkTokenType( const TokenMap& tokenMap, const std::string& token )
{
	TokenMap::const_iterator	it;

	it = tokenMap.find(token);
	if (it == tokenMap.end())
		return (STRING);
	return (it->second);
}

std::list<Token*>	tokenizer( std::ifstream& file )
{
	std::list<Token*>	output;
	TokenMap			tokenMap;
	std::string			line;
	std::string			word;

	initMap(tokenMap);
	while (std::getline(file, line))
	{
		std::istringstream	stream(line);
		while (!stream.eof())
		{
			stream >> word;
			output.push_back(new Token(checkTokenType(tokenMap, word), word));
		}
	}
	for (std::list<Token*>::iterator iter = output.begin(); iter != output.end(); iter++)
	{
		std::cout << **iter << std::endl;
	}
	return (output);
}
