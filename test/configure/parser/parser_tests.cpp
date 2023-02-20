#include "gtest/gtest.h"
#include "../../../src/configure/parser/Token.hpp"
#include "../../../src/configure/Node.hpp"
#include "../../../src/configure/parser/parser.hpp"
#include "../../../src/configure/parser/TokenStream.hpp"

TEST(parseServer, client_max_body_size)
{
	std::list<Token> lst = {
		Token(T_CLIENT_MAX_BODY, "client_max_body_size", 1),
		Token(T_STRING, "22", 1),
		Token(T_SEMICOLON, ";", 1)};
	TokenStream testInput(lst);

	Node*	output = parseClientMaxBodySize(testInput);
	ASSERT_TRUE(output != NULL);
	NodeList::const_iterator i = output->getChildrenBegin();
	EXPECT_EQ((*i)->getNodeType(), TERMINAL);
	EXPECT_EQ((*i)->getTerminal(), "22");
}

TEST(parseServer, index)
{
	std::list<Token> lst = {
 		Token(T_INDEX, "index", 1),
		Token(T_STRING, "index.html", 1),
		Token(T_STRING, "index.htm", 1),
		Token(T_STRING, "index.php", 1),
		Token(T_SEMICOLON, ";", 1)};
	TokenStream	testInput(lst);

	Node*	output = parseIndex(testInput);
	ASSERT_TRUE(output != NULL);
	NodeList::const_iterator i = output->getChildrenBegin();
	EXPECT_EQ((*i)->getNodeType(), TERMINAL);
	EXPECT_EQ((*i)->getTerminal(), "index.html");
	++i;
	EXPECT_EQ((*i)->getNodeType(), TERMINAL);
	EXPECT_EQ((*i)->getTerminal(), "index.htm");
	++i;
	EXPECT_EQ((*i)->getNodeType(), TERMINAL);
	EXPECT_EQ((*i)->getTerminal(), "index.php");
	EXPECT_EQ(testInput.isEmpty(), true);
}

TEST(parseServer, serverName)
{
	std::list<Token> lst = {
		Token(T_SERVER_NAME, "server_name", 1),
		Token(T_STRING, "www.website.org", 1),
		Token(T_SEMICOLON, ";", 1)};
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
	std::list<Token> lst = {
		Token(T_LISTEN, "listen", 1),
		Token(T_STRING, "80", 1),
		Token(T_SEMICOLON, ";", 1)};
	TokenStream testInput(lst);

	Node*	output = parseListen(testInput);
	ASSERT_TRUE(output != NULL);
	NodeList::const_iterator i = output->getChildrenBegin();
	EXPECT_EQ((*i)->getNodeType(), TERMINAL);
	EXPECT_EQ((*i)->getTerminal(), "80");
	EXPECT_EQ(testInput.isEmpty(), true);
}

TEST(parseServer, errorPage)
{
	std::list<Token> lst = {
		Token(T_ERROR_PAGE, "error_page", 1),
		Token(T_STRING, "404", 1),
		Token(T_STRING, "/custom_404.html", 1),
		Token(T_SEMICOLON, ";", 1)};
	TokenStream testInput(lst);

	Node*	output = parseErrorPage(testInput);
	ASSERT_TRUE(output != NULL);
	NodeList::const_iterator i = output->getChildrenBegin();
	EXPECT_EQ((*i)->getNodeType(), TERMINAL);
	EXPECT_EQ((*i)->getTerminal(), "404");
	++i;
	EXPECT_EQ((*i)->getNodeType(), TERMINAL);
	EXPECT_EQ((*i)->getTerminal(), "/custom_404.html");
	EXPECT_EQ(testInput.isEmpty(), true);
}

TEST(parseServer, parseServer)
{
	std::list<Token> lst = {
		Token(T_SERVER, "server", 1),
		Token(T_BRACKET_OPEN, "{", 1),
		Token(T_SERVER_NAME, "server_name", 1),
		Token(T_STRING, "www.test.org", 1),
		Token(T_SEMICOLON, ";", 1),
		Token(T_LISTEN, "listen", 1),
		Token(T_STRING, "80", 1),
		Token(T_SEMICOLON, ";", 1),
		Token(T_BRACKET_CLOSE, "}", 1)};
	TokenStream testInput(lst);

	Node*	output = parseServer(testInput);
	ASSERT_TRUE(output != NULL);
	NodeList::const_iterator i = output->getChildrenBegin();
	EXPECT_EQ((*i)->getNodeType(), N_SERVER_NAME);
	++i;
	EXPECT_EQ((*i)->getNodeType(), N_LISTEN);
	EXPECT_EQ(testInput.isEmpty(), true);
}

TEST(parser, basic_2_servers)
{	
	std::list<Token> lst = {
		Token(T_SERVER, "server", 1),
		Token(T_BRACKET_OPEN, "{", 1),
		Token(T_SERVER_NAME, "server_name", 1),
		Token(T_STRING, "www.test.org", 1),
		Token(T_SEMICOLON, ";", 1),
		Token(T_LISTEN, "listen", 1),
		Token(T_STRING, "80", 1),
		Token(T_SEMICOLON, ";", 1),
		Token(T_BRACKET_CLOSE, "}", 1),
		Token(T_SERVER, "server", 1),
		Token(T_BRACKET_OPEN, "{", 1),
		Token(T_SERVER_NAME, "server_name", 1),
		Token(T_STRING, "www.test.org", 1),
		Token(T_SEMICOLON, ";", 1),
		Token(T_LISTEN, "listen", 1),
		Token(T_STRING, "80", 1),
		Token(T_SEMICOLON, ";", 1),
		Token(T_BRACKET_CLOSE, "}", 1)};
	TokenStream testInput(lst);

	Node*	output = parser(testInput);
	ASSERT_TRUE(output != NULL);
	NodeList::const_iterator i = output->getChildrenBegin();
	EXPECT_EQ((*i)->getNodeType(), N_SERVER);
	++i;
	EXPECT_EQ((*i)->getNodeType(), N_SERVER);
	EXPECT_EQ(testInput.isEmpty(), true);
}
