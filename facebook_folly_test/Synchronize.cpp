#include <iostream>
#include <thread>
#include <functional>
#include <folly/Synchronized.h>
#include <vector>
#include <queue>
#include <mutex>
#include <chrono>
#include <condition_variable>

using namespace std;
using namespace std::chrono_literals;

int main(){

    folly::Synchronized< queue<int>, std::mutex > sync_queue;

    thread reader1([&sync_queue](){
        while(true){
            int front = -1;
            do{
                {
                    auto _queue = sync_queue.lock();
                    if(!_queue->empty()){
                        front = _queue->front();
                        _queue->pop();
                    }
                }
                //this_thread::sleep_for(100ms);
            }while(front == -1);
            cout << "thread1 = " << front << endl;
            front = -1;
        }
    });
    reader1.detach();

    thread reader2([&sync_queue](){
        while(true){
            int front = -1;
            while(front == -1){
                sync_queue.withLock([&front](auto& _queue){
                    if(!_queue.empty())
                    {
                        front = _queue.front();
                        _queue.pop();
                    }
                    //this_thread::sleep_for(100ms);
                });
            }
            cout << "thread2 = " <<front << endl;

        }
    });
    reader2.detach();

    thread writer([&sync_queue](){
        int item = 0;
        while(true){
            {
                auto _queue = sync_queue.lock();
                _queue->push(item++);
            }
            this_thread::sleep_for(1s);
        }
    });
    writer.join();

    return 0;
}
