#include <list>
#include <vector>
#include "../../../src/configure/parser/parser.hpp"
#include "../../../src/configure/parser/Token.hpp"
#include "../../../src/configure/parser/tokenizer.hpp"
#include "../../../src/configure/parser/TokenStream.hpp"
#include "../../../src/configure/Node.hpp"

#include "gtest/gtest.h"

TEST(parseLocation_integration, basic_location)
{
	std::ifstream file("dummy_config/location_test.conf");
	TokenStream* testInput = tokenizer(file);

	Node*	output = parseLocation(*testInput);
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

TEST(parseLocation_integration, valid_case)
{
	std::ifstream file("dummy_config/parseLocation_test_1");
	TokenStream* testInput = tokenizer(file);

	Node*	output = parseLocation(*testInput);
	NodeList::const_iterator i = output->getChildrenBegin();
	ASSERT_TRUE(output != NULL);
	EXPECT_EQ((*i)->getNodeType(), TERMINAL);
	EXPECT_EQ((*i)->getTerminal(), "/");
	++i;
	EXPECT_EQ((*i)->getNodeType(), N_INDEX);
	++i;
	EXPECT_EQ((*i)->getNodeType(), N_ALLOWED_METHODS);
	++i;
	EXPECT_EQ((*i)->getNodeType(), N_AUTOINDEX);
}
