
#include "gtest/gtest.h"
#include "Token.hpp"
#include "tokenizer.hpp"

TEST(checkTokenType_tests, basic)
{
	TokenMap	tokenMap;

	initMap(tokenMap);
	EXPECT_EQ(checkTokenType(tokenMap, "server"), SERVER);
}
