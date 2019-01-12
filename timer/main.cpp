
#include <memory>
#include <iostream>
#include <vector>
#include "timer.h"
#include "timerQueue.h"

void print_use_count(int n){
    cout << "wkp use_count = " << n << endl;
}

void tvshift(struct timeval &tv, long milliSec) {

	tv.tv_usec += ( milliSec % 1000) * 1000;
	tv.tv_sec  += int( milliSec / 1000);

	tv.tv_sec += int(tv.tv_usec / 1000000);
	tv.tv_usec = tv.tv_usec % 1000000;
}

int main(){
    //boost::asio::io_service io;
    //timer mytimer(io);
    timer mytimer;
    mytimer.start();
    int expire = 10000;
    struct timeval tv;
    gettimeofday(&tv, NULL);
    tvshift(tv, expire);
    timeEvent te(tv.tv_sec, tv.tv_usec);
    weak_ptr<int> wkp;
    print_use_count(wkp.use_count());
    {
        shared_ptr<int> count = make_shared<int>(10);
        wkp = count;
        print_use_count(wkp.use_count());
        callBackFunction print_func = [count](){ cout << "left callback = " << count.use_count() << endl; };
        print_use_count(wkp.use_count());
        mytimer.post_timestamp(te,print_func);
        mytimer.post_duration(10,0,[count](){ cout << "right callback = " << count.use_count() << endl; });
        print_use_count(wkp.use_count());
    }
    print_use_count(wkp.use_count());
    while(true){
        sleep(2);
        print_use_count(wkp.use_count());
    }
    return 0;
}

