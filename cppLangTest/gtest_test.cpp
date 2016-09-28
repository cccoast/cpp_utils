
#include <iostream>
#include "gtest/gtest.h"

using namespace std;

TEST(sample_test_case, sample_test)
{
    EXPECT_EQ(1, 1);
}

int main(int argc, char** argv)
{
    testing::InitGoogleTest(&argc, argv);
    RUN_ALL_TESTS();
    return 0;
}
