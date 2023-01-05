
#include "gtest/gtest.h"
#include "tokenizer.hpp"

TEST(tokenizer_tests, basic)
{
	std::ifstream	file("file");

	tokenizer(file);
}