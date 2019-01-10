//##################################################################################
//###@author xudi                                                                  #
//###@Mail   xudi1989@ruc.edu.cn                                                   #
//###@Lisence GPL                                                                  #
//##################################################################################
#include <folly/futures/Future.h>
#include <iostream>

using namespace std;

int pftest(){

    folly::Promise<int> p;
    folly::Future<int> action = p.getFuture();
    action.then([](int x){ cout << x << endl;});
    p.setValue(100);
    return 0;
}
