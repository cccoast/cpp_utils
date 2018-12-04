//##################################################################################
//###@author xudi                                                                  #
//###@Mail   xudi1989@ruc.edu.cn                                                   #
//###@Lisence GPL                                                                  #
//##################################################################################

#include <iostream>
#include "impl_test.h"

using namespace std;

int main(){

    {
        impl_test<int> storage;
        storage.set_attr(10);
        cout << storage.get_attr() << endl;
    }
    cout << endl;

    {
        impl_test<int> storage;
        int v = 11;
        storage.set_attr(v);
        cout << storage.get_attr() << endl;
    }
    cout << endl;

    {
        impl_test<int> storage;
        int v2 = 12;
        storage.set_attr(std::move(v2));
        cout << storage.get_attr() << endl;
    }

    return 0;
}
