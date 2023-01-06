
#include "gtest/gtest.h"
#include "tokenizer.hpp"
#include <fstream>

TEST(tokenizer_tests, basic)
{
	std::ifstream	file("nginx.conf");

	tokenizer(file);
}

TEST(tokenizer_tests, checkTokenType)
{
	TokenMap	tokenMap;

	initMap(tokenMap);
	EXPECT_EQ(checkTokenType(tokenMap, "server"), SERVER);
}