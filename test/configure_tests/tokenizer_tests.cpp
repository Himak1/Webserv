
#include "gtest/gtest.h"
#include "tokenizer.hpp"
#include <fstream>

bool	string_eq(std::string a, std::string b)
{
	return (a == b);
}

TEST(createTokenList_tests, double_quotes)
{
	std::ifstream	file("dummy_config/comments.conf");
	std::list<Token*>	tokenList = createTokenList(file);


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
	EXPECT_EQ(checkTokenType(tokenMap, "server"), T_SERVER);
}

TEST(splitLine_tests, basic_string)
{
	std::string	input("this is a line");
	std::list<std::string>	expected_words = {"this", "is", "a", "line"};

	std::list<std::string>	words = splitLineByDelimiters(input);
	EXPECTED_EQ(words, expected_words);
}

TEST(splitLine_tests, line_with_comment)
{
	std::string	input("line { } #comment");
	std::list<std::string>	expected_words = {"line", "{", "}"};

	std::list<std::string>	words = splitLineByDelimiters(input);
	EXPECT_EQ(words, expected_words);
}

TEST(splitLine_tests, multiple_delimiters_in_a_row)
{
	std::string	input("line 	{  }#comment");
	std::list<std::string> expected_words = {"line", "{", "}"};

	std::list<std::string>	words = splitLineByDelimiters(input);
	EXPECT_EQ(words, expected_words);
}

TEST(splitLine_tests, semicolons)
{
	std::string	input("alias /usr/bin/; allowed_methods GET;");
	std::list<std::string>	expected_words = {"alias", "/usr/bin/", ";", "allowed_methods", "GET", ";"};

	std::list<std::string>	words = splitLineByDelimiters(input);
	EXPECT_EQ(words, expected_words);
}

TEST(splitLine_tests, semicolons_in_a_row)
{
	std::string	input("alias /usr/bin/;;; allowed_methods GET;");
	std::list<std::string>	expected_words = {"alias", "/usr/bin/", ";", ";", ";", "allowed_methods", "GET", ";"};

	std::list<std::string>	words = splitLineByDelimiters(input);
	EXPECT_EQ(words, expected_words);
}
