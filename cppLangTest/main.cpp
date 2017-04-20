#include <iostream>
#include <stdio.h>
#include <memory>
#include <gtest/gtest.h>

using namespace std;

/*int main(){

    int argc = 2;
    char* argv[] = { "ALL_TEST","--gtest_filter=PRETTY_PRINT.*"};
    testing::InitGoogleTest(&argc,argv);
    return RUN_ALL_TESTS();
}
*/

int main(){

    int a = 1, b = 100;
    int sum = 0;

    for (int i = 0; i < b; i ++)
        sum += i;

    cout << sum << endl;
    return 0;
}
