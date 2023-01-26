#include "gtest/gtest.h"
#include "Token.hpp"
#include "Node.hpp"
#include "parser.hpp"

TEST(parseLocation, location)
{
	TokenList	tList;
	TokenList::iterator	iter;
	std::ifstream file("dummy_config/location_test.conf");

	tList = tokenizer(file);
	iter = tList.begin();

	Node* output = parseLocation(iter, tList.end());
	EXPECT_TRUE(output != nullptr);
}

TEST(parseLocation, locationPath)
{
	TokenList	tList;
	tList.push_back(new Token(STRING, "/usr/share/"));
	TokenList::iterator iter = tList.begin();
	TokenList::iterator end = tList.end();

	Node*	output;
	output = parseLocationPath(iter, end);
	EXPECT_TRUE(output != nullptr);
}

TEST(parseLocation, alias)
{
	// setup
	TokenList	tList;
	tList.push_back(new Token(ALIAS, "alias"));
	tList.push_back(new Token(STRING, "/usr/bin/"));
	tList.push_back(new Token(SEMICOLON, ";"));
	TokenList::iterator iter = tList.begin();
	TokenList::iterator end = tList.end();

	Node*	output;
	output = parseLocationAlias(iter, end);
	EXPECT_TRUE(output != nullptr);
	if (output)
		delete output;

	tList.insert(tList.back(), new Token(STRING, "garbage"));
	iter = tList.begin();
	end = tList.end();
	output = parseLocationAlias(iter, end);
	EXPECT_TRUE(output == nullptr);

	for (auto it = tList.begin(); it != tList.end(); ++it)
		delete *it;
}
