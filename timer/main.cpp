
#include <memory>
#include <iostream>
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
    boost::asio::io_service io;
    timer mytimer(io);
    mytimer.start();
    int expire = 5000;
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
        callBackFunction print_func = [count](){ cout << "callback = " << count.use_count() << ' ' << *count << endl; };
        print_use_count(wkp.use_count());
        cout << "hi" << endl;
        mytimer.post(te,print_func);
        cout << "hi" << endl;
        mytimer.post(te,[count](){ cout << "callback = " << count.use_count() << ' ' << *count << endl; });
        print_use_count(wkp.use_count());
    }
    print_use_count(wkp.use_count());
    while(true){
        sleep(4);
        cout << wkp.use_count() << endl;
    }
    return 0;
}

/*
class timerTest{
public:
	static int count;
	void run(void* arg) {
		cout << "count = " << count << ", pointer = " << *(int *)arg << endl;
		count++;
	}
	void deconstruct(void* arg) {
		cout << "deconstruct" << endl;
		if (arg)
			free(arg);
	}
};
int timerTest::count = 0;

int main(){
    timerTest tt;
    int expire = 3000;
    int tid = 1;
	boost::asio::io_service io;
	timer timer_test(io, boost::bind(&timerTest::run, &tt, _1), boost::bind(&timerTest::deconstruct, &tt, _1));
	timer_test.start();
	struct timeval tv;
    gettimeofday(&tv, NULL);
    tvshift(tv, expire);
    int* p = (int *)malloc(sizeof(int));
    *p = tid;
    timeEvent te(tv.tv_sec, tv.tv_usec);
    te.arg = p;
    timer_test.post(te);
    while(true){
        sleep(10);
    }
    delete p;
}
*/
