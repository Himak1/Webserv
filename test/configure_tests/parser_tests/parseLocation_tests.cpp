#include "gtest/gtest.h"
#include "Token.hpp"
#include "Node.hpp"
#include "parser.hpp"

TEST(parseLocation, AllowedMethods)
{
	TokenList	lst;
	lst.push_back(new Token(T_ALLOWED_METHODS, "allowed_methods"));
	lst.push_back(new Token(T_STRING, "GET"));
	lst.push_back(new Token(T_STRING, "POST"));
}

TEST(parseLocation, locationPath)
{
	TokenList	tList;
	tList.push_back(new Token(T_STRING, "/usr/share/"));
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
	tList.push_back(new Token(T_ALIAS, "alias"));
	tList.push_back(new Token(T_STRING, "/usr/bin/"));
	tList.push_back(new Token(T_SEMICOLON, ";"));
	TokenList::iterator iter = tList.begin();
	TokenList::iterator end = tList.end();

	Node*	output;
	output = parseLocationAlias(iter, end);
	EXPECT_TRUE(output != nullptr);
	if (output)
		delete output;
	tList.clear();

	TokenList	lst;
	lst.push_back(new Token(T_ALIAS, "alias"));
	lst.push_back(new Token(T_STRING, "/usr/bin/"));
	lst.push_back(new Token(T_STRING, "garbage"));
	lst.push_back(new Token(T_SEMICOLON, ";"));
	iter = lst.begin();
	end = lst.end();
	output = parseLocationAlias(iter, end);
}

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

