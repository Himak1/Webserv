
#include <iostream>
#include <sstream>
#include <string>
#include <map>
#include "tokenizer.hpp"
#include "Token.hpp"

void	initMap( std::map<std::string, int>& tokenMap )
{
	tokenMap["server"]			= T_SERVER;
	tokenMap["server_name"]		= T_SERVER_NAME;
	tokenMap["listen"] 			= T_LISTEN;
	tokenMap["port"] 			= T_PORT;
	tokenMap["access_log"]		= T_ACCESS_LOG;
	tokenMap["location"]		= T_LOCATION;
	tokenMap["root"]			= T_ROOT;
	tokenMap["expires"] 		= T_EXPIRES;
	tokenMap["error_page"]		= T_ERROR_PAGE;
	tokenMap["return"]			= T_RETURN;
	tokenMap["allowed_methods"]	= T_ALLOWED_METHODS;
	tokenMap["cgi_pass"]		= T_CGI_PASS;
	tokenMap["alias"]			= T_ALIAS;
	// Punctuation and Seperators
	tokenMap["{"]				= T_BRACKET_OPEN;
	tokenMap["}"]				= T_BRACKET_CLOSE;
	tokenMap["/"]				= T_FORWARD_SLASH;
	tokenMap["\\"]				= T_BACK_SLASH;
	tokenMap[";"]				= T_SEMICOLON;
	tokenMap["#"]				= T_HASHTAG;
	tokenMap["$"]				= T_DOLLAR;
	tokenMap["~"]				= T_HOME_DIR;
}

int	checkTokenType( const TokenMap& tokenMap, const std::string& token )
{
	TokenMap::const_iterator	it;

	it = tokenMap.find(token);
	if (it == tokenMap.end())
		return (T_STRING);
	return (it->second);
}

bool	isEnd(std::string::iterator it)
{
	if (*it == ' '
		|| *it == '\t'
		|| *it == '\v'
		|| *it == ';')
		return true;
	return false;
}

std::list<std::string>	splitLine( std::string line )
{
	std::list<std::string>	words;
	size_t	start = 0;
	size_t	ending = 0;

	while ( start != std::string::npos && start != line.size() && line[start] != '#')
	{
		ending = line.find_first_of(" \t\v;#", start);
		if (ending - start != 0)
			words.push_back(line.substr(start, ending - start));
		else if (line[start] == ';')
		{
			words.push_back(line.substr(start, ending - start + 1));
			ending++;
			start = ending;
			continue ;
		}
		if (ending != std::string::npos && line[ending] == '#')
			break ;
		ending = line.find_first_not_of(" \t\v", ending);
		start = ending;
	}
	return (words);
}

std::list<Token*>	tokenizer( std::ifstream& file )
{
	std::list<Token*>		output;
	std::list<std::string>	words;
	TokenMap				tokenMap;
	std::string				line;

	initMap(tokenMap);
	while (std::getline(file, line))
	{
		words = splitLine(line);
		for (std::list<std::string>::iterator it = words.begin(); it != words.end(); ++it)
		{
			if ((*it)[0] == '#')
				break ;
			output.push_back(new Token(checkTokenType(tokenMap, *it), *it));
		}
	}
	// for (std::list<Token*>::iterator iter = output.begin(); iter != output.end(); iter++)
	// {
	// 	std::cout << **iter << std::endl;
	// }
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
// 			if (output.back()->getTokenType() == HASHTAG)
// 				break ;
// 		}
// 	}
// 	// for (std::list<Token*>::iterator iter = output.begin(); iter != output.end(); iter++)
// 	// {
// 	// 	std::cout << **iter << std::endl;
// 	// }
// 	return (output);
// }
