#include <list>
#include <vector>
#include "parser.hpp"
#include "Token.hpp"
#include "Node.hpp"

#include "gtest/gtest.h"

TEST(parseLocation_integration, location)
{
	TokenList	tList;
	TokenList::iterator	iter;
	TokenList::iterator	ending;
	std::ifstream file("dummy_config/location_test.conf");

	tList = tokenizer(file);
	iter = tList.begin();
	ending = tList.end();

	Node* output = parseLocation(iter, ending);
	EXPECT_TRUE(output != nullptr);
}
