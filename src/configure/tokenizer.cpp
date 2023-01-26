
#include <iostream>
#include <sstream>
#include <string>
#include <map>
#include "tokenizer.hpp"
#include "Token.hpp"

void	initMap( std::map<std::string, int>& tokenMap )
{
	tokenMap["server"]			= SERVER;
	tokenMap["server_name"]		= SERVER_NAME;
	tokenMap["listen"] 			= LISTEN;
	tokenMap["port"] 			= PORT;
	tokenMap["access_log"]		= ACCESS_LOG;
	tokenMap["location"]		= LOCATION;
	tokenMap["root"]			= ROOT;
	tokenMap["expires"] 		= EXPIRES;
	tokenMap["error_page"]		= ERROR_PAGE;
	tokenMap["return"]			= RETURN;
	tokenMap["allowed_methods"]	= ALLOWED_METHODS;
	tokenMap["cgi_pass"]		= CGI_PASS;
	tokenMap["alias"]			= ALIAS;
	// Punctuation and Seperators
	tokenMap["{"]				= BRACKET_OPEN;
	tokenMap["}"]				= BRACKET_CLOSE;
	tokenMap["/"]				= FORWARD_SLASH;
	tokenMap["\\"]				= BACK_SLASH;
	tokenMap[";"]				= SEMICOLON;
	tokenMap["#"]				= HASHTAG;
	tokenMap["$"]				= DOLLAR;
	tokenMap["~"]				= HOME_DIR;
}

int	checkTokenType( const TokenMap& tokenMap, const std::string& token )
{
	TokenMap::const_iterator	it;

	it = tokenMap.find(token);
	if (it == tokenMap.end())
		return (STRING);
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
		if (line[ending] == '#')
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
