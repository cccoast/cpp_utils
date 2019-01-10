//##################################################################################
//###@author xudi                                                                  #
//###@Mail   xudi1989@ruc.edu.cn                                                   #
//###@Lisence GPL                                                                  #
//##################################################################################

#include <iostream>
#include <memory>
#include <functional>
#include <thread>
#include <chrono>
#include <vector>

#include <unistd.h>

#include <folly/FBString.h>
#include <folly/FBVector.h>>
#include <folly/Foreach.h>

using namespace std;

int fbstring_test(){

    folly::fbstring s("xudi");
    s.append(",hello");
    s = "abcabcdabcdefg";

    folly::fbstring toberep("a");
    folly::fbstring rep("x");
    auto iter = 0;
    while( ( iter = s.find(toberep,iter) ) >= 0){
        cout << iter << " -> " << s[iter] << endl;
        s.replace(iter,toberep.length(),rep);
        cout << s << endl;
    }
    cout << s << endl;
    return 0;
}

