#include "gtest/gtest.h"
#include "Token.hpp"
#include "TokenStream.hpp"
#include "Node.hpp"
#include "parser.hpp"

TEST(parseLocation, AllowedMethods)
{
	std::list<Token*> lst = {	new Token(T_ALLOWED_METHODS, "allowed_methods"),
								new Token(T_STRING, "GET"),
								new Token(T_STRING, "POST"),
								new Token(T_SEMICOLON, ";")};
	TokenStream	testInput(lst);

	Node*	output = parseAllowedMethods(testInput);
	ASSERT_TRUE(output != NULL);
	NodeList::const_iterator i = output->getChildrenBegin();
	EXPECT_EQ((*i)->getNodeType(), TERMINAL);
	EXPECT_EQ((*i)->getTerminal(), "GET");
}

TEST(parseLocation, locationPath)
{
	std::list<Token*> lst = {	new Token(T_STRING, "/usr/share/")};
	TokenStream	testInput(lst);

	Node*	output = parseLocationPath(testInput);
	ASSERT_TRUE(output != NULL);
	EXPECT_EQ(output->getTerminal(), "/usr/share/");
}

TEST(parseLocation, root)
{
	std::list<Token*> lst = {
		new Token(T_ROOT, "root"),
		new Token(T_STRING, "/files/data"),
		new Token(T_SEMICOLON, ";")};
	TokenStream	testInput(lst);

	Node*	output = parseRoot(testInput);
	ASSERT_TRUE(output != NULL);
	NodeList::const_iterator i = output->getChildrenBegin();
	EXPECT_EQ((*i)->getNodeType(), TERMINAL);
	EXPECT_EQ((*i)->getTerminal(), "/files/data");
}

TEST(parseLocation, cgi_pass)
{
	std::list<Token*> lst = {
		new Token(T_CGI_PASS, "cgi_pass"),
		new Token(T_STRING, ".php"),
		new Token(T_STRING, "/usr/bin/php"),
		new Token(T_SEMICOLON, ";")};
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

/*
TEST(parseLocation, alias)
{
	std::list<Token*> lst = {
		new Token(T_ALIAS, "alias"),
		new Token(T_STRING, "/usr/bin/"),
		new Token(T_SEMICOLON, ";")};
	TokenStream testInput(lst);
}
*/

/* TEST(parseLocation, alias) */
/* { */
/* 	// setup */
/* 	TokenList	tList; */
/* 	tList.push_back(new Token(T_ALIAS, "alias")); */
/* 	tList.push_back(new Token(T_STRING, "/usr/bin/")); */
/* 	tList.push_back(new Token(T_SEMICOLON, ";")); */
/* 	TokenList::iterator iter = tList.begin(); */
/* 	TokenList::iterator end = tList.end(); */

/* 	Node*	output; */
/* 	output = parseAlias(iter, end); */
/* 	ASSERT_TRUE(output != nullptr); */
/* 	if (output) */
/* 		delete output; */
/* 	tList.clear(); */

/* 	TokenList	lst; */
/* 	lst.push_back(new Token(T_ALIAS, "alias")); */
/* 	lst.push_back(new Token(T_STRING, "/usr/bin/")); */
/* 	lst.push_back(new Token(T_STRING, "garbage")); */
/* 	lst.push_back(new Token(T_SEMICOLON, ";")); */
/* 	iter = lst.begin(); */
/* 	end = lst.end(); */
/* 	output = parseAlias(iter, end); */
/* } */

