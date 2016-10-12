//##################################################################################
//###@author xudi                                                                  #
//###@Mail   xudi1989@ruc.edu.cn                                                   #
//###@Lisence GPL                                                                  #
//##################################################################################

#include <iostream>
#include <malloc.h>

using namespace std;

int jemalloc_test(){
    int *p = (int *)malloc(sizeof(int));
    *p = 20;
    cout << *p << endl;
    return 0;
}
