
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
	EXPECT_PRED2(string_eq, (*iter)->getToken(), "#");
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
	EXPECT_EQ(words[0], "this");
	EXPECT_EQ(words[1], "is");
	EXPECT_EQ(words[2], "a");
	EXPECT_EQ(words[3], "line");
}
