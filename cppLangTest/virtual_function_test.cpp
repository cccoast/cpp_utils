#include <iostream>
#include <memory>
#include <memory.h>

using namespace std;

class test{
public:
    test(){ cout << "base construct " << endl;}
    virtual ~test(){ cout << "base deconstruct " << endl;}
};

class sub_test: public test{
    int sub_place_holder;
public:
    sub_test():sub_place_holder(0){ cout << "sub construct" << endl;}
    ~sub_test() final { cout << "sub deconstruct" << endl;}
};

int virtual_test(){

    test* t1 = new sub_test;
    test* t2 = (test *)malloc(sizeof(sub_test));
    test* t3 = new(t2) sub_test;
    delete t3;
    delete t1;
    return 0;
}
