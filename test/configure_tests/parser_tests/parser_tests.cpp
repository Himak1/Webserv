
#include "gtest/gtest.h"
#include "Token.hpp"
#include "Node.hpp"
#include "parser.hpp"

TEST(parser_tests, location)
{
	TokenList	tList;
	TokenList::iterator	iter;
	std::ifstream file("dummy_config/location_test.conf");

	tList = tokenizer(file);
	iter = tList.begin();

	Node* output = parseLocation(iter, tList.end());
	EXPECT_TRUE(output != nullptr);
}

TEST(parser_tests, locationPath)
{
	TokenList	tList;
	tList.push_back(new Token(STRING, "/usr/share/"));
	TokenList::iterator iter = tList.begin();
	TokenList::iterator end = tList.end();

	Node*	output;
	output = parseLocation(iter, end);
	EXPECT_TRUE(output != nullptr);
}