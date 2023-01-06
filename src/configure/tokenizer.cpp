
#include <iostream>
#include <sstream>
#include <string>
#include <map>
#include "tokenizer.hpp"
#include "Token.hpp"

void	initMap( std::map<std::string, int>& tokenMap )
{
	tokenMap["server"]		= SERVER;
	tokenMap["server_name"]	= SERVER_NAME;
	tokenMap["listen"] 		= LISTEN;
	tokenMap["port"] 		= PORT;
	tokenMap["access_log"]	= ACCESS_LOG;
	tokenMap["location"]	= LOCATION;
	tokenMap["root"]		= ROOT;
	tokenMap["expires"] 	= EXPIRES;
	tokenMap["error_page"]	= ERROR_PAGE;
	tokenMap["{"]			= BRACKET_OPEN;
	tokenMap["}"]			= BRACKET_CLOSE;
	tokenMap["/"]			= FORWARD_SLASH;
	tokenMap["\\"]			= BACK_SLASH;
	tokenMap[";"]			= SEMICOLON;
	tokenMap["#"]			= HASHTAG;
	tokenMap["$"]			= DOLLAR;
	tokenMap["~"]			= HOME_DIR;
}

int	checkTokenType( const TokenMap& tokenMap, const std::string& token )
{
	TokenMap::const_iterator	it;

	it = tokenMap.find(token);
	if (it == tokenMap.end())
		return (STRING);
	return (it->second);
}

std::string	stringSplit( std::istringstream& stream )
{

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
		word = stringSplit(stream);
		if (!stream.eof())
		output.push_back(new Token(checkTokenType(tokenMap, word), word));
	}
	for (std::list<Token*>::iterator iter = output.begin(); iter != output.end(); iter++)
	{
		// std::cout << **iter << std::endl;
	}
	return (output);
}

// std::list<Token*>	tokenizer( std::ifstream& file )
// {
// 	std::list<Token*>	output;
// 	TokenMap			tokenMap;
// 	std::string			line;
// 	std::string			word;

// 	initMap(tokenMap);
// 	while (std::getline(file, line))
// 	{
// 		std::istringstream	stream(line);
// 		while (!stream.eof())
// 		{
// 			stream >> word;
// 			output.push_back(new Token(checkTokenType(tokenMap, word), word));
// 		}
// 	}
// 	for (std::list<Token*>::iterator iter = output.begin(); iter != output.end(); iter++)
// 	{
// 		// std::cout << **iter << std::endl;
// 	}
// 	return (output);
// }
