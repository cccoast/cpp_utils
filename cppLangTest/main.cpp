#include <iostream>
#include <stdio.h>
#include <memory>
#include <gtest/gtest.h>

using namespace std;

int main(){

    int argc = 2;
    char* argv[] = { "ALL_TEST","--gtest_filter=PRETTY_PRINT.*"};
    testing::InitGoogleTest(&argc,argv);
    return RUN_ALL_TESTS();
}
