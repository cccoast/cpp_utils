#include <iostream>
#include <stdio.h>
#include <memory>

using namespace std;

int new_at_specific_position(){

    int p;
    new (&p) int(10);
    cout << p << endl;
    return 0;
}
