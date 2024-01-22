
#include "ipaddr.h"

#include <gtest/gtest.h>
#include <string>
#include <vector>


namespace {

  TEST(Google_test, test_ipaddr) {
    auto ip1 = ipaddr("10.11.12.13");
    EXPECT_EQ(ip1.uint(), (10U<<24U)+(11U<<16U)+(12U<<8U)+13);
    EXPECT_EQ(ip1.str(), "10.11.12.13");
    EXPECT_THROW(ipaddr("10.11.12;13"),std::logic_error);
    EXPECT_THROW(ipaddr("10.11.12.266"),std::logic_error);
  }

  TEST(Google_test, test_compiler) {
    EXPECT_EQ(ipaddr::bytesize(), 8);
    EXPECT_EQ(ipaddr::maxbyte(), 255);
  }
}
