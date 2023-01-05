
#include <iostream>
#include <sstream>
#include <string>
#include "tokenizer.hpp"
#include "Token.hpp"

static void	initMap( std::map<string, int>& tokenTypes )
{
	tokenTypes["server_name"]	= SERVER_NAME;
	tokenTypes["listen"] 		= LISTEN;
	tokenTypes["port"] 			= PORT;
	tokenTypes["access_log"]	= ACCESS_LOG;
	tokenTypes["location"]		= LOCATION;
	tokenTypes["root"]			= ROOT;
	tokenTypes["expires"] 		= EXPIRES;
	tokenTypes["error_page"]	= ERROR_PAGE;
	tokenTypes["{"]				= BRACKET_OPEN;
	
}

std::vector<Token>	tokenizer( std::ifstream& file )
{
	std::vector<Token>		output;
	std::map<string, int>	tokenTypes;
	std::string				line;
	std::string				word;

	initMap()
	while (std::getline(file, line))
	{
		std::istringstream	stream(line);
		stream >> word;
		if (matchToToken(word) == -1)

		std::cout << word << std::endl;
	}
	return (output);
}
