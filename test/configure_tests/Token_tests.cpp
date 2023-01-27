
#include "Token.hpp"
#include "gtest/gtest.h"

TEST(Token_tests, getTokenType)
{
	Token	t1(T_STRING, "string");

	EXPECT_EQ(t1.getTokenType(), T_STRING);
}

TEST(Token_tests, getToken)
{
	Token	t1(T_SEMICOLON, ";");

	EXPECT_EQ(t1.getTokenType(), T_SEMICOLON);
	EXPECT_TRUE(t1.getToken() == ";");
}
