#include <iostream>
#include <thread>
#include <functional>
#include <folly/Synchronized.h>
#include <vector>
#include <queue>
#include <mutex>
#include <chrono>
#include <condition_variable>
#include <gtest/gtest.h>

using namespace std;
using namespace std::chrono_literals;

TEST(Synchronized,MCMP){

    bool stopFlag = false;
    folly::Synchronized< queue<int>, std::mutex > sync_queue;

    thread reader1([&sync_queue,&stopFlag](){
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
                if(stopFlag) return ;
            }while(front == -1);
            cout << "thread1 = " << front << endl;
            front = -1;

        }
    });

    thread reader2([&sync_queue,&stopFlag](){
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
                if(stopFlag) return ;
            }
            cout << "thread2 = " <<front << endl;
        }
    });

    thread writer([&sync_queue,&stopFlag](){
        int item = 0;
        while(true){
            {
                auto _queue = sync_queue.lock();
                _queue->push(item++);
            }
            this_thread::sleep_for(1s);
            if(item == 5){
                stopFlag = true;
                return ;
            }
        }
    });
    writer.join();
    reader1.join();
    reader2.join();
    EXPECT_EQ(true,stopFlag);
}

TEST(Synchronized,MCMP_ConditionVariable){

    bool stopFlag = false;
    folly::Synchronized< queue<int>, std::mutex > sync_queue;
    condition_variable empty_cv;

    thread reader1([&sync_queue,&stopFlag,&empty_cv](){
        while(true){
            int front = -1;
            {
                auto _queue = sync_queue.lock();
                empty_cv.wait(_queue.getUniqueLock(),[&_queue,&stopFlag](){
                    return !_queue->empty() || stopFlag;
                });
                if(!_queue->empty()){
                    front = _queue->front();
                    _queue->pop();
                }
            }
            if(stopFlag) return ;
            else{
                cout << "thread1 = " << front << endl;
                front = -1;
            }
        }
    });

    thread reader2([&sync_queue,&stopFlag,&empty_cv](){
        while(true){
            int front = -1;
            {
                auto _queue = sync_queue.lock();
                empty_cv.wait(_queue.getUniqueLock(),[&_queue,&stopFlag](){
                    return !_queue->empty() || stopFlag;
                });
                if(!_queue->empty()){
                    front = _queue->front();
                    _queue->pop();
                }
            }
            if(stopFlag) return ;
            else{
                cout << "thread2 = " << front << endl;
                front = -1;
            }
        }
    });


    thread writer([&sync_queue,&stopFlag,&empty_cv](){
        int item = 0;
        while(true){
            {
                auto _queue = sync_queue.lock();
                _queue->push(item++);
                empty_cv.notify_one();
            }
            this_thread::sleep_for(1s);
            if(item == 20){
                stopFlag = true;
                empty_cv.notify_all();
                return ;
            }
        }
    });
    writer.join();
    reader1.join();
    reader2.join();
    EXPECT_EQ(true,stopFlag);
}


int SynchronizeTest(int argc,char* argv[]){
    testing::InitGoogleTest(&argc,argv);
    return RUN_ALL_TESTS();
}
