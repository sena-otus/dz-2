
#include "split.h"

#include <gtest/gtest.h>
#include <string>
#include <vector>



namespace {

  TEST(Google_test, test_version) {
    std::vector<std::string> rezult = {"",""};
    EXPECT_EQ(split(".", '.'), rezult);
  }
}
