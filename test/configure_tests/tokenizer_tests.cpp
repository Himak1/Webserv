
#include "gtest/gtest.h"
#include "tokenizer.hpp"
#include <fstream>

bool	string_eq(std::string a, std::string b)
{
	return (a == b);
}

TEST(tokenizer_tests, double_quotes)
{
	std::ifstream	file("dummy_config/comments.conf");

	std::list<Token*>	tokenList = tokenizer(file);
	std::list<Token*>::iterator	iter = tokenList.begin();
	EXPECT_PRED2(string_eq, (*iter)->getToken(), "server");
	iter++;
	EXPECT_PRED2(string_eq, (*iter)->getToken(), "{");
	iter++;
	EXPECT_PRED2(string_eq, (*iter)->getToken(), "location");
	iter++;
	EXPECT_PRED2(string_eq, (*iter)->getToken(), "{");
	iter++;
	EXPECT_PRED2(string_eq, (*iter)->getToken(), "}");
	iter++;
	EXPECT_PRED2(string_eq, (*iter)->getToken(), "}");
	iter++;
	EXPECT_TRUE(iter == tokenList.end());
}

TEST(tokenizer_tests, checkTokenType)
{
	TokenMap	tokenMap;

	initMap(tokenMap);
	EXPECT_EQ(checkTokenType(tokenMap, "server"), SERVER);
}

TEST(splitLine_tests, basic_string)
{
	std::string	input("this is a line");

	std::list<std::string>	words = splitLine(input);
	std::list<std::string>::iterator it = words.begin();
	EXPECT_EQ(*it, "this");
	++it;
	EXPECT_EQ(*it, "is");
	++it;
	EXPECT_EQ(*it, "a");
	++it;
	EXPECT_EQ(*it, "line");
}

TEST(splitLine_tests, line_with_comment)
{
	std::string	input("line { } #comment");

	std::list<std::string>	words = splitLine(input);
	std::list<std::string>::iterator it = words.begin();
	EXPECT_EQ(*it, "line");
	++it;
	EXPECT_EQ(*it, "{");
	++it;
	EXPECT_EQ(*it, "}");
	++it;
	EXPECT_EQ(it, words.end());
}

TEST(splitLine_tests, multiple_delimiters_in_a_row)
{
	std::string	input("line 	{  }#comment");

	std::list<std::string>	words = splitLine(input);
	std::list<std::string>::iterator it = words.begin();
	EXPECT_EQ(*it, "line");
	++it;
	EXPECT_EQ(*it, "{");
	++it;
	EXPECT_EQ(*it, "}");
	++it;
	EXPECT_EQ(it, words.end());
}

TEST(splitLine_tests, semicolons)
{
	std::string	input("alias /usr/bin/; allowed_methods GET;");

	std::list<std::string>	words = splitLine(input);
	std::list<std::string>::iterator it = words.begin();
	EXPECT_EQ(*it, "alias");
	++it;
	EXPECT_EQ(*it, "/usr/bin/");
	++it;
	EXPECT_EQ(*it, ";");
	++it;
	EXPECT_EQ(*it, "allowed_methods");
	++it;
	EXPECT_EQ(*it, "GET");
	++it;
	EXPECT_EQ(*it, ";");
	++it;
	EXPECT_EQ(it, words.end());
}

TEST(splitLine_tests, semicolons_in_a_row)
{
	std::string	input("alias /usr/bin/;;; allowed_methods GET;");

	std::list<std::string>	words = splitLine(input);
	std::list<std::string>::iterator it = words.begin();
	EXPECT_EQ(*it, "alias");
	++it;
	EXPECT_EQ(*it, "/usr/bin/");
	++it;
	EXPECT_EQ(*it, ";");
	++it;
	EXPECT_EQ(*it, ";");
	++it;
	EXPECT_EQ(*it, ";");
	++it;
	EXPECT_EQ(*it, "allowed_methods");
	++it;
	EXPECT_EQ(*it, "GET");
	++it;
	EXPECT_EQ(*it, ";");
	++it;
	EXPECT_EQ(it, words.end());
}