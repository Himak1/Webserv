#include <iostream>
#include <cstdlib>
#include "configure.hpp"
#include "Node.hpp"
#include "parser/TokenStream.hpp"
#include "parser/Token.hpp"
#include "parser/tokenizer.hpp"
#include "parser/parser.hpp"

static void	deleteAllElementsInVector( vector<Configuration*> vec )
{
	for (vector<Configuration*>::iterator i = vec.begin(); i != vec.end(); ++i)
	{
		delete *i;
	}
}

static void	openConfigFile( ifstream& configFile, int argc, char **argv )
{
	if (argc >= 2)
	{
		configFile.open(argv[1]);
		if (!configFile)
		{
			cerr << "ERROR: '" << argv[1] << "' could not be opened" << endl;
			exit(1);
		}
	}
	else
	{
		configFile.open("default.conf");
		if (!configFile)
		{
			cerr << "ERROR: default configuration file wasn't found" << endl;
			exit(1);
		}
	}
}

static vector<Configuration*>	convertASTtoConfigVector( Node* ast )
{
	vector<Configuration*>	serverConfigs;

	for (NodeList::const_iterator i = ast->getChildrenBegin(); i != ast->getChildrenEnd(); ++i) {
		serverConfigs.push_back(new Configuration(*i));
	}
	return (serverConfigs);	
}

vector<Configuration*>	initializeConfigurations( int argc, char **argv )
{
	TokenStream*	tokens;
	Node*			ast;
	ifstream	configFile;
	bool			exceptionCaught;

	exceptionCaught = false;
	openConfigFile(configFile, argc, argv);
	tokens = tokenizer(configFile);
	if (tokens->isEmpty()) {
		cerr << "ERROR: Empty config file" << endl;
		exit(1);
	}
	ast = parser(*tokens);
	if (!ast) {
		delete tokens;
		exit(1);
	}
	vector<Configuration*>	serverConfigs;
	try {
		serverConfigs = convertASTtoConfigVector(ast);
	}
	catch (exception& e) {
		cerr << "ERROR: " << e.what() << '\n';
		deleteAllElementsInVector(serverConfigs);
		exceptionCaught = true;
	}
	delete ast;
	delete tokens;
	if (exceptionCaught) exit(1);
	return (serverConfigs);
}

