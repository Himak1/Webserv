
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

std::vector<Token>	tokenizer( std::ifstream& file )
{
	std::vector<Token>			output;
	std::map<std::string, int>	tokenMap;
	std::string					line;
	std::string					word;

	initMap(tokenMap);
	while (std::getline(file, line))
	{
		std::istringstream	stream(line);
		stream >> word;
		output.push_back(Token(checkTokenType(tokenMap, word), word));
	}
	return (output);
}
