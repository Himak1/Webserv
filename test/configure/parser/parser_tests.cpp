#include "gtest/gtest.h"
#include "Token.hpp"
#include "Node.hpp"
#include "parser.hpp"
#include "TokenStream.hpp"

TEST(parseServer, client_max_body_size)
{
	std::list<Token*> lst = {
		new Token(T_CLIENT_MAX_BODY, "client_max_body_size"),
		new Token(T_STRING, "22"),
		new Token(T_SEMICOLON, ";")};
	TokenStream testInput(lst);

	Node*	output = parseClientMaxBodySize(testInput);
	ASSERT_TRUE(output != NULL);
	NodeList::const_iterator i = output->getChildrenBegin();
	EXPECT_EQ((*i)->getNodeType(), TERMINAL);
	EXPECT_EQ((*i)->getTerminal(), "22");
}

TEST(parseServer, index)
{
	std::list<Token*> lst = {
		new Token(T_INDEX, "index"),
		new Token(T_STRING, "index.html"),
		new Token(T_STRING, "index.htm"),
		new Token(T_STRING, "index.php"),
		new Token(T_SEMICOLON, ";")};
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
	std::list<Token*> lst = {
		new Token(T_SERVER_NAME, "server_name"),
		new Token(T_STRING, "www.website.org"),
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
	std::list<Token*> lst = {
		new Token(T_ERROR_PAGE, "error_page"),
		new Token(T_STRING, "404"),
		new Token(T_STRING, "/custom_404.html"),
		new Token(T_SEMICOLON, ";")};
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
	std::list<Token*> lst = {
		new Token(T_SERVER, "server"),
		new Token(T_BRACKET_OPEN, "{"),
		new Token(T_SERVER_NAME, "server_name"),
		new Token(T_STRING, "www.test.org"),
		new Token(T_SEMICOLON, ";"),
		new Token(T_LISTEN, "listen"),
		new Token(T_STRING, "80"),
		new Token(T_SEMICOLON, ";"),
		new Token(T_BRACKET_CLOSE, "}")};
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
	std::list<Token*> lst = {
		new Token(T_SERVER, "server"),
		new Token(T_BRACKET_OPEN, "{"),
		new Token(T_SERVER_NAME, "server_name"),
		new Token(T_STRING, "www.test.org"),
		new Token(T_SEMICOLON, ";"),
		new Token(T_LISTEN, "listen"),
		new Token(T_STRING, "80"),
		new Token(T_SEMICOLON, ";"),
		new Token(T_BRACKET_CLOSE, "}"),
		new Token(T_SERVER, "server"),
		new Token(T_BRACKET_OPEN, "{"),
		new Token(T_SERVER_NAME, "server_name"),
		new Token(T_STRING, "www.test.org"),
		new Token(T_SEMICOLON, ";"),
		new Token(T_LISTEN, "listen"),
		new Token(T_STRING, "80"),
		new Token(T_SEMICOLON, ";"),
		new Token(T_BRACKET_CLOSE, "}")};
	TokenStream testInput(lst);

	Node*	output = parser(testInput);
	ASSERT_TRUE(output != NULL);
	NodeList::const_iterator i = output->getChildrenBegin();
	EXPECT_EQ((*i)->getNodeType(), N_SERVER);
	++i;
	EXPECT_EQ((*i)->getNodeType(), N_SERVER);
	EXPECT_EQ(testInput.isEmpty(), true);
}
