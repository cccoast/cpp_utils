#include <iostream>
#include <memory>
#include <functional>
#include <thread>
#include <chrono>

#include <unistd.h>

#include <folly/ProducerConsumerQueue.h>
#include <folly/FBString.h>

using namespace std;

void show(folly::fbstring& s){
    cout << s << endl;
}

int produceConsumerTest(){

    folly::ProducerConsumerQueue<folly::fbstring> _queue(10000);

    std::thread reader([&_queue] {
      for (;;) {
        folly::fbstring str;
        while (!_queue.read(str)) {
          //spin until we get a value
          continue;
        }
        cout << "recv!" << endl;
        show(str);
      }
    });

    // producer thread:
    for (;;) {
      folly::fbstring str = "hello";
      while (_queue.write(str)) {
        //spin until the queue has room
        this_thread::sleep_for(std::chrono::seconds(1));
        cout << "send!" << endl;
      }
    }


    return 0;
}
