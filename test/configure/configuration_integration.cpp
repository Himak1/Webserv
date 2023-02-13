#include "gtest/gtest.h"
#include "../../src/configure/parsing/tokenizer.hpp"
#include "../../src/configure/parsing/parser.hpp"
#include "../../src/configure/parsing/TokenStream.hpp"
#include "../../src/configure/Node.hpp"
#include "../../src/configure/Configuration.hpp"

TEST(configuration_integration, test_1)
{
	std::ifstream	file("dummy_config/configuration_integration_1");

	TokenStream	testInput = tokenizer(file);
	Node*		ast = parser(testInput);

	ASSERT_TRUE(ast != NULL);
	NodeList::const_iterator i = ast->getChildrenBegin();
	EXPECT_EQ((*i)->getNodeType(), N_SERVER);
	ASSERT_TRUE(*i != NULL);
	Configuration* tmp = new Configuration(*i);
	(void)tmp;
}