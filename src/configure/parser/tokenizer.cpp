#include <iostream>
#include <sstream>
#include <string>
#include <map>
#include "tokenizer.hpp"
#include "TokenStream.hpp"
#include "Token.hpp"

void	initMap( std::map<std::string, int>& tokenMap )
{
	tokenMap["server"]			= T_SERVER;
	tokenMap["server_name"]		= T_SERVER_NAME;
	tokenMap["listen"] 			= T_LISTEN;
	tokenMap["index"]			= T_INDEX;
	tokenMap["client_max_body_size"] = T_CLIENT_MAX_BODY;
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
	tokenMap["autoindex"]		= T_AUTOINDEX;
	// Punctuation and Seperators
	tokenMap["{"]				= T_BRACKET_OPEN;
	tokenMap["}"]				= T_BRACKET_CLOSE;
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

std::list<std::string>	splitLineByDelimiters( std::string line )
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

std::list<Token>	createTokensFromStrings( std::list<std::string> splitStrings, TokenMap tokenMap, int lineCount )
{
	std::list<Token>	tokensFromLine;

	for (std::list<std::string>::iterator it = splitStrings.begin(); it != splitStrings.end(); ++it)
	{
		if ((*it)[0] == '#')
			break ;
		tokensFromLine.push_back(Token(checkTokenType(tokenMap, *it), *it, lineCount));
	}
	return (tokensFromLine);
}

std::list<Token>	createTokenList( std::ifstream& file, TokenMap tokenMap )
{
	std::list<Token>	tokenList;
	std::string			line;
	int					lineCount = 1;

	while (std::getline(file, line))
	{
		std::list<std::string>	splitStrings = splitLineByDelimiters(line);
		std::list<Token> tokensFromLine = createTokensFromStrings(splitStrings, tokenMap, lineCount);
		tokenList.splice(tokenList.end(), tokensFromLine);
		lineCount++;
	}
	return (tokenList);
}

TokenStream	tokenizer( std::ifstream& file )
{
	TokenMap				tokenMap;

	initMap(tokenMap);
	std::list<Token>	tokenList = createTokenList(file, tokenMap);
	TokenStream	tokenStream(tokenList);
	return (tokenStream);
}
