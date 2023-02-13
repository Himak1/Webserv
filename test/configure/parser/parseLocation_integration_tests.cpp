#include <list>
#include <vector>
#include "../../../src/configure/parsing/parser.hpp"
#include "../../../src/configure/parsing/Token.hpp"
#include "../../../src/configure/parsing/tokenizer.hpp"
#include "../../../src/configure/parsing/TokenStream.hpp"
#include "../../../src/configure/Node.hpp"

#include "gtest/gtest.h"

TEST(parseLocation_integration, basic_location)
{
	std::ifstream file("dummy_config/location_test.conf");
	TokenStream testInput = tokenizer(file);

	Node*	output = parseLocation(testInput);
	NodeList::const_iterator i = output->getChildrenBegin();
	ASSERT_TRUE(output != NULL);
	EXPECT_EQ((*i)->getNodeType(), TERMINAL);
	EXPECT_EQ((*i)->getTerminal(), "/data/files/");
	++i;
	EXPECT_EQ((*i)->getNodeType(), N_ALIAS);
	++i;
	EXPECT_EQ((*i)->getNodeType(), N_ROOT);
	++i;
	EXPECT_EQ((*i)->getNodeType(), N_AUTOINDEX);
	++i;
	EXPECT_EQ((*i)->getNodeType(), N_ALLOWED_METHODS);
	++i;
	EXPECT_EQ((*i)->getNodeType(), N_CGI_PASS);
}

