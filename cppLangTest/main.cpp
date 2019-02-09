#include <memory>
#include <iostream>

using namespace std;

class Base: public std::enable_shared_from_this<Base>{
public:
    int get_use_count(){
        shared_ptr<Base> sptr = shared_from_this();
        return sptr.use_count();
    }
};

class Derived: public Base{
};

int main(){
    shared_ptr<Base> base = make_shared<Base>();
    cout << base.use_count() << endl;
    cout << base->get_use_count() << endl;
    cout << base.use_count() << endl;

    auto derived = make_shared<Derived>();
    cout << derived.use_count() << endl;
    cout << derived->get_use_count() << endl;
    cout << derived.use_count() << endl;
    return 0;
}

