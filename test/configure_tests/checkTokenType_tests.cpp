
#include "gtest/gtest.h"
#include "Token.hpp"
#include "tokenizer.hpp"

TEST(assignTokenType_tests, basic)
{
	std::map<std::string, int>	tokenMap;

	initMap(tokenMap);
	EXPECT_EQ(checkTokenType(tokenMap, "server"), SERVER);
}
