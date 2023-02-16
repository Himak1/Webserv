#include <iostream>
#include "configure.hpp"
#include "Node.hpp"
#include "parser/TokenStream.hpp"
#include "parser/Token.hpp"
#include "parser/tokenizer.hpp"
#include "parser/parser.hpp"

static void	openConfigFile( std::ifstream& configFile, int argc, char **argv )
{
	if (argc >= 2)	configFile.open(argv[1]);
	else 			configFile.open("default.conf");
	if (!configFile) std::terminate();
}

static std::list<Configuration*>	convertASTtoConfigList( Node* ast )
{
	std::list<Configuration*>	serverConfigs;

	for (NodeList::const_iterator i = ast->getChildrenBegin(); i != ast->getChildrenEnd(); ++i) {
		try {
			serverConfigs.push_back(new Configuration(*i));
		}
		catch (std::exception& e) {
			std::cout << "invalid values were found" << std::endl;
			throw std::exception();
		}
	}
	return (serverConfigs);
	
}

std::list<Configuration*>	initializeConfigurations( int argc, char **argv )
{
	std::ifstream	configFile;
	openConfigFile(configFile, argc, argv);

	TokenStream					tokens = tokenizer(configFile);
	Node*						ast = parser(tokens);
	if (!ast)
		std::terminate();
	std::list<Configuration*>	serverConfigs = convertASTtoConfigList(ast);
	return (serverConfigs);
}

