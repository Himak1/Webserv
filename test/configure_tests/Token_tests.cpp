
#include "Token.hpp"
#include "gtest/gtest.h"

TEST(Token_tests, getTokenType)
{
	Token	t1;
	EXPECT_EQ(t1.getTokenType(), INVALID_TOKEN);
}

TEST(Token_tests, getToken)
{
	Token	t1(SEMICOLON, ";");

	EXPECT_EQ(t1.getTokenType(), SEMICOLON);
	EXPECT_TRUE(t1.getToken() == ";");
}
