#include "gtest/gtest.h"
#include "../../../src/configure/parsing/Token.hpp"
#include "../../../src/configure/parsing/TokenStream.hpp"
#include "../../../src/configure/Node.hpp"
#include "../../../src/configure/parsing/parser.hpp"

TEST(parseLocation, parseReturn)
{
	std::list<Token> lst = {	Token(T_RETURN, "return"),
								Token(T_STRING, "301"),
								Token(T_STRING, "localhost/"),
								Token(T_SEMICOLON, ";")};
	TokenStream	testInput(lst);

	Node*	output = parseReturn(testInput);
	ASSERT_TRUE(output != NULL);
	NodeList::const_iterator i = output->getChildrenBegin();
	EXPECT_EQ((*i)->getNodeType(), TERMINAL);
	EXPECT_EQ((*i)->getTerminal(), "301");
	++i;
	EXPECT_EQ((*i)->getNodeType(), TERMINAL);
	EXPECT_EQ((*i)->getTerminal(), "localhost/");	
	EXPECT_EQ(testInput.isEmpty(), true);
}

TEST(parseLocation, AllowedMethods)
{
	std::list<Token> lst = {	Token(T_ALLOWED_METHODS, "allowed_methods"),
								Token(T_STRING, "GET"),
								Token(T_STRING, "POST"),
								Token(T_SEMICOLON, ";")};
	TokenStream	testInput(lst);

	Node*	output = parseAllowedMethods(testInput);
	ASSERT_TRUE(output != NULL);
	NodeList::const_iterator i = output->getChildrenBegin();
	EXPECT_EQ((*i)->getNodeType(), TERMINAL);
	EXPECT_EQ((*i)->getTerminal(), "GET");
	++i;
	EXPECT_EQ((*i)->getNodeType(), TERMINAL);
	EXPECT_EQ((*i)->getTerminal(), "POST");
	EXPECT_EQ(testInput.isEmpty(), true);
}

TEST(parseLocation, locationPath)
{
	std::list<Token> lst = {Token(T_STRING, "/usr/share/")};
	TokenStream	testInput(lst);

	Node*	output = parseLocationPath(testInput);
	ASSERT_TRUE(output != NULL);
	EXPECT_EQ(output->getTerminal(), "/usr/share/");
	EXPECT_EQ(testInput.isEmpty(), true);
}

TEST(parseLocation, root)
{
	std::list<Token> lst = {
		Token(T_ROOT, "root"),
		Token(T_STRING, "/files/data"),
		Token(T_SEMICOLON, ";")};
	TokenStream	testInput(lst);

	Node*	output = parseRoot(testInput);
	ASSERT_TRUE(output != NULL);
	NodeList::const_iterator i = output->getChildrenBegin();
	EXPECT_EQ((*i)->getNodeType(), TERMINAL);
	EXPECT_EQ((*i)->getTerminal(), "/files/data");
	EXPECT_EQ(testInput.isEmpty(), true);
}

TEST(parseLocation, cgi_pass)
{
	std::list<Token> lst = {
		Token(T_CGI_PASS, "cgi_pass"),
		Token(T_STRING, ".php"),
		Token(T_STRING, "/usr/bin/php"),
		Token(T_SEMICOLON, ";")};
	TokenStream testInput(lst);

	Node*	output = parseCgiPass(testInput);
	ASSERT_TRUE(output != NULL);
	NodeList::const_iterator i = output->getChildrenBegin();
	EXPECT_EQ((*i)->getNodeType(), TERMINAL);
	EXPECT_EQ((*i)->getTerminal(), ".php");
	++i;
	EXPECT_EQ((*i)->getNodeType(), TERMINAL);
	EXPECT_EQ((*i)->getTerminal(), "/usr/bin/php");
	EXPECT_EQ(testInput.isEmpty(), true);
}

TEST(parseLocation, alias)
{
	std::list<Token> lst = {
		Token(T_ALIAS, "alias"),
		Token(T_STRING, "/usr/bin/"),
		Token(T_SEMICOLON, ";")};
	TokenStream testInput(lst);

	Node*	output = parseAlias(testInput);
	NodeList::const_iterator i = output->getChildrenBegin();
	ASSERT_TRUE(output != NULL);
	EXPECT_EQ((*i)->getNodeType(), TERMINAL);
	EXPECT_EQ((*i)->getTerminal(), "/usr/bin/");
	EXPECT_EQ(testInput.isEmpty(), true);
}

TEST(parseLocation, autoindex)
{
	std::list<Token> lst = {
		Token(T_AUTOINDEX, "autoindex"),
		Token(T_STRING, "on"),
		Token(T_SEMICOLON, ";")};
	TokenStream testInput(lst);

	Node*	output = parseAutoIndex(testInput);
	NodeList::const_iterator i = output->getChildrenBegin();
	ASSERT_TRUE(output != NULL);
	EXPECT_EQ((*i)->getNodeType(), TERMINAL);
	EXPECT_EQ((*i)->getTerminal(), "on");
	EXPECT_EQ(testInput.isEmpty(), true);
}
