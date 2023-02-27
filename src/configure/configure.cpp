#include <iostream>
#include <cstdlib>
#include "configure.hpp"
#include "Node.hpp"
#include "parser/TokenStream.hpp"
#include "parser/Token.hpp"
#include "parser/tokenizer.hpp"
#include "parser/parser.hpp"

static void	deleteAllElementsInVector( std::vector<Configuration*> vec )
{
	for (std::vector<Configuration*>::iterator i = vec.begin(); i != vec.end(); ++i)
	{
		delete *i;
	}
}

static void	openConfigFile( std::ifstream& configFile, int argc, char **argv )
{
	if (argc >= 2)
	{
		configFile.open(argv[1]);
		if (!configFile)
		{
			std::cerr << "ERROR: '" << argv[1] << "' could not be opened" << std::endl;
			std::exit(1);
		}
	}
	else
	{
		configFile.open("default.conf");
		if (!configFile)
		{
			std::cerr << "ERROR: default configuration file wasn't found" << std::endl;
			std::exit(1);
		}
	}
}

static std::vector<Configuration*>	convertASTtoConfigVector( Node* ast )
{
	std::vector<Configuration*>	serverConfigs;

	for (NodeList::const_iterator i = ast->getChildrenBegin(); i != ast->getChildrenEnd(); ++i) {
		serverConfigs.push_back(new Configuration(*i));
	}
	return (serverConfigs);	
}

std::vector<Configuration*>	initializeConfigurations( int argc, char **argv )
{
	TokenStream*	tokens;
	Node*			ast;
	std::ifstream	configFile;
	bool			exceptionCaught;

	exceptionCaught = false;
	openConfigFile(configFile, argc, argv);
	tokens = tokenizer(configFile);
	ast = parser(*tokens);
	if (!ast) {
		delete tokens;
		std::exit(1);
	}
	std::vector<Configuration*>	serverConfigs;
	try {
		serverConfigs = convertASTtoConfigVector(ast);
	}
	catch (std::exception& e) {
		std::cerr << "ERROR: " << e.what() << '\n';
		deleteAllElementsInVector(serverConfigs);
		exceptionCaught = true;
	}
	delete ast;
	delete tokens;
	if (exceptionCaught) std::exit(1);
	return (serverConfigs);
}

