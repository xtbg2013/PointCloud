#include "gtest/gtest.h"
#include <iostream>
using namespace std;
using namespace testing;


TEST(CASE1,zero)
{
    EXPECT_EQ(0,0);
}

int main(int argc,char* argv[])
{
    testing::InitGoogleTest(&argc,argv);
    int ret = RUN_ALL_TESTS();
    return ret;
}