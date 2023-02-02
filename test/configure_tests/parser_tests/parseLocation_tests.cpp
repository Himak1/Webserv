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
	ASSERT_TRUE(output != nullptr);
	NodeList::const_iterator i = output->getChildrenBegin();
	EXPECT_EQ((*i)->getNodeType(), TERMINAL);
	EXPECT_EQ((*i)->getTerminal(), "GET");
}

/* TEST(parseLocation, locationPath) */
/* { */
/* 	TokenList	tList; */
/* 	tList.push_back(new Token(T_STRING, "/usr/share/")); */
/* 	TokenList::iterator iter = tList.begin(); */
/* 	TokenList::iterator end = tList.end(); */

/* 	Node*	output; */
/* 	output = parseLocationPath(iter, end); */
/* 	ASSERT_TRUE(output != nullptr); */
/* } */

/* TEST(parseLocation, root) */
/* { */
/* 	TokenList	lst; */
/* 	lst.push_back(new Token(T_ROOT, "/usr/share/")); */
/* 	lst.push_back(new Token(T_STRING, "/files/data")); */
/* 	lst.push_back(new Token(T_SEMICOLON, ";")); */
/* 	TokenList::iterator iter = lst.begin(); */
/* 	TokenList::iterator ending = lst.end(); */

/* 	Node*	output; */
/* 	output = parseRoot(iter, ending); */
/* 	ASSERT_TRUE(output != nullptr); */
/* } */

/* TEST(parseLocation, cgi_pass) */
/* { */
/* 	TokenList	lst; */
/* 	lst.push_back(new Token(T_CGI_PASS, "cgi_pass")); */
/* 	lst.push_back(new Token(T_STRING, ".php")); */
/* 	lst.push_back(new Token(T_STRING, "/usr/bin/php")); */
/* 	lst.push_back(new Token(T_SEMICOLON, ";")); */
/* 	TokenList::iterator iter = lst.begin(); */
/* 	TokenList::iterator ending = lst.end(); */

/* 	Node*	output; */
/* 	output = parseCgiPass(iter, ending); */
/* 	ASSERT_TRUE(output != nullptr); */
/* } */

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

