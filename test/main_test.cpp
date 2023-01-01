#include "gtest/gtest.h"

int g_exit_code = 0;

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}