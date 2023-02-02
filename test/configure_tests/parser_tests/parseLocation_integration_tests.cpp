#include <list>
#include <vector>
#include "parser.hpp"
#include "Token.hpp"
#include "tokenizer.hpp"
#include "Node.hpp"

#include "gtest/gtest.h"

/* TEST(parseLocation_integration, location) */
/* { */
/* 	std::list<Token*>	tList; */
/* 	std::list<Token*>::iterator	iter; */
/* 	std::list<Token*>::iterator	ending; */
/* 	std::ifstream file("dummy_config/location_test.conf"); */

/* 	tList = tokenizer(file); */
/* 	iter = tList.begin(); */
/* 	ending = tList.end(); */

/* 	Node* output = parseLocation(iter, ending); */
/* 	EXPECT_TRUE(output != nullptr); */
/* } */
