#include "../../src/configure/parser/Token.hpp"
#include "gtest/gtest.h"

TEST(Token_tests, getTokenType)
{
	Token	t1(T_STRING, "string", 1);

	EXPECT_EQ(t1.getTokenType(), T_STRING);
}

TEST(Token_tests, getToken)
{
	Token	t1(T_SEMICOLON, ";", 1);

	EXPECT_EQ(t1.getTokenType(), T_SEMICOLON);
	EXPECT_TRUE(t1.getToken() == ";");
}
