
#include "gtest/gtest.h"
#include "tokenizer.hpp"
#include <fstream>

TEST(tokenizer_tests, basic)
{
	std::ifstream	file("Makefile");

	tokenizer(file);
}