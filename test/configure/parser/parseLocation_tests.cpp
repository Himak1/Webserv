#include "gtest/gtest.h"
#include "../../../src/configure/parser/Token.hpp"
#include "../../../src/configure/parser/TokenStream.hpp"
#include "../../../src/configure/Node.hpp"
#include "../../../src/configure/parser/parser.hpp"

TEST(parseLocation, parseReturn)
{
	std::list<Token> lst = {	Token(T_RETURN, "return", 1),
								Token(T_STRING, "301", 1),
								Token(T_STRING, "localhost/", 1),
								Token(T_SEMICOLON, ";", 1)};
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
	std::list<Token> lst = {	Token(T_ALLOWED_METHODS, "allowed_methods", 1),
								Token(T_STRING, "GET", 1),
								Token(T_STRING, "POST", 1),
								Token(T_SEMICOLON, ";", 1)};
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
	std::list<Token> lst = {Token(T_STRING, "/usr/share/", 1)};
	TokenStream	testInput(lst);

	Node*	output = parseLocationPath(testInput);
	ASSERT_TRUE(output != NULL);
	EXPECT_EQ(output->getTerminal(), "/usr/share/");
	EXPECT_EQ(testInput.isEmpty(), true);
}

TEST(parseLocation, root)
{
	std::list<Token> lst = {
		Token(T_ROOT, "root", 1),
		Token(T_STRING, "/files/data", 1),
		Token(T_SEMICOLON, ";", 1)};
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
		Token(T_CGI_PASS, "cgi_pass", 1),
		Token(T_STRING, ".php", 1),
		Token(T_STRING, "/usr/bin/php", 1),
		Token(T_SEMICOLON, ";", 1)};
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
		Token(T_ALIAS, "alias", 1),
		Token(T_STRING, "/usr/bin/", 1),
		Token(T_SEMICOLON, ";", 1)};
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
		Token(T_AUTOINDEX, "autoindex", 1),
		Token(T_STRING, "on", 1),
		Token(T_SEMICOLON, ";", 1)};
	TokenStream testInput(lst);

	Node*	output = parseAutoIndex(testInput);
	NodeList::const_iterator i = output->getChildrenBegin();
	ASSERT_TRUE(output != NULL);
	EXPECT_EQ((*i)->getNodeType(), TERMINAL);
	EXPECT_EQ((*i)->getTerminal(), "on");
	EXPECT_EQ(testInput.isEmpty(), true);
}
