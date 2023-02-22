#include "gtest/gtest.h"
#include "../../src/configure/parser/tokenizer.hpp"
#include "../../src/configure/parser/parser.hpp"
#include "../../src/configure/parser/TokenStream.hpp"
#include "../../src/configure/Node.hpp"

TEST(tokenizer_parser, valid_server_all_params)
{
	std::ifstream file("dummy_config/complete_server.conf");

	TokenStream*	testInput = tokenizer(file);
	Node*	output = parser(*testInput);

	ASSERT_TRUE(output != NULL);
	NodeList::const_iterator i = output->getChildrenBegin();
	EXPECT_EQ((*i)->getNodeType(), N_SERVER);
	++i;
	EXPECT_EQ((*i)->getNodeType(), N_SERVER);
	EXPECT_EQ(testInput->isEmpty(), true);
}


TEST(tokenizer_parser, syntax_error)
{
	std::ifstream file("dummy_config/complete_syntax_error.conf");

	TokenStream*	testInput = tokenizer(file);
	Node*	output = parser(*testInput);

	ASSERT_TRUE(output == NULL);
}
