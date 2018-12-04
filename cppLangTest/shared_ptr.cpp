//##################################################################################
//###@author xudi                                                                  #
//###@Mail   xudi1989@ruc.edu.cn                                                   #
//###@Lisence GPL                                                                  #
//##################################################################################
#include <iostream>
#include <memory>

using namespace std;

int count = 0;

class test{
public:
    int _count;
    test(){ _count = count++; cout << "construct " << _count << endl;}
    ~test(){ cout << "deconstruct " << _count << endl;}
};

void test_weak_ptr(){
    shared_ptr<test> sp = make_shared<test>();
    cout << sp.use_count() << endl;
    cout << "sub field begin" << endl;
    {
        weak_ptr<test> sp2 = sp;
        cout << sp.use_count() << endl;
    }
    cout << "sub field end" << endl;
    cout << sp.use_count() << endl;
    return ;
}

void test_shared_ptr(){
    shared_ptr<test> sp = make_shared<test>();
    cout << sp.use_count() << endl;
    cout << "sub field begin" << endl;
    {
        shared_ptr<test> sp2 = sp;
        cout << sp.use_count() << endl;
    }
    cout << "sub field end" << endl;
    cout << sp.use_count() << endl;
}


int shared_ptr_test(){

    cout << "--->>> test weak\n"<<endl;
    test_weak_ptr();

    cout << "--->>> test shared\n"<<endl;
    test_shared_ptr();

    return 0;
}
