
#include "gtest/gtest.h"
#include "Token.hpp"
#include "Node.hpp"
#include "parser.hpp"

TEST(parser_tests, location)
{
	TokenList	tList;
	TokenList::iterator	iter;
	std::ifstream file("dummy_config/location_test.conf");

	tList = tokenizer(file);
	iter = tList.begin();

	Node* output = parseLocation(tList, iter);
	EXPECT_TRUE(output != nullptr);
}