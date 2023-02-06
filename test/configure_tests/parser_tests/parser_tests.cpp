#include "gtest/gtest.h"
#include "Token.hpp"
#include "Node.hpp"
#include "parser.hpp"
#include "TokenStream.hpp"

TEST(parseServer, serverName)
{
	std::list<Token*> lst = {
		new Token(T_SERVER_NAME, "server_name"),
		new Token(T_STRING, "www.website.org");
		new Token(T_SEMICOLON, ";")};
	TokenStream	testInput(lst);

	Node*	output = parseServerName(testInput);
	ASSERT_TRUE(output != NULL);
	NodeList::const_iterator i = output->getChildrenBegin();
	EXPECT_EQ((*i)->getNodeType(), TERMINAL);
	EXPECT_EQ((*i)->getTerminal(), "www.website.org");
	EXPECT_EQ(testInput.isEmpty(), true);
}

TEST(parseServer, listen)
{
	std::list<Token*> lst = {
		new Token(T_LISTEN, "listen"),
		new Token(T_STRING, "80"),
		new Token(T_SEMICOLON, ";")};
	TokenStream(lst);

	Node*	output;
	
}

/* TEST(parseServer, parseServer) */
/* { */
/* 	std::list<Token*> lst = { */
/* 		new Token(T_ */
/* } */
