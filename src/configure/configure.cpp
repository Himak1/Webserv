#include <iostream>
#include <cstdlib>
#include "configure.hpp"
#include "Node.hpp"
#include "parser/TokenStream.hpp"
#include "parser/Token.hpp"
#include "parser/tokenizer.hpp"
#include "parser/parser.hpp"

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
		try {
			serverConfigs.push_back(new Configuration(*i));
		}
		catch (std::exception& e) {
			std::cout	<< "ERROR: invalid values were found in the configuration file"
						<< std::endl;
			std::exit(1);
		}
	}
	return (serverConfigs);
	
}

std::vector<Configuration*>	initializeConfigurations( int argc, char **argv )
{
	std::ifstream	configFile;
	openConfigFile(configFile, argc, argv);

	TokenStream		tokens = tokenizer(configFile);
	Node*			ast = parser(tokens);
	if (!ast)
		exit(1);
	std::vector<Configuration*>	serverConfigs = convertASTtoConfigVector(ast);
	return (serverConfigs);
}

