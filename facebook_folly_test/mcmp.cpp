
#include <folly/MPMCQueue.h>
#include <folly/Format.h>
#include <folly/Memory.h>
#include <folly/Foreach.h>
#include <folly/portability/GTest.h>
#include <folly/portability/SysResource.h>
#include <folly/portability/SysTime.h>
#include <folly/portability/Unistd.h>
#include <folly/test/DeterministicSchedule.h>

#include <boost/intrusive_ptr.hpp>
#include <memory>
#include <functional>
#include <thread>
#include <utility>
#include <atomic>
#include <chrono>

FOLLY_ASSUME_FBVECTOR_COMPATIBLE_1(boost::intrusive_ptr);

using namespace folly;
using namespace detail;
using namespace test;
using std::chrono::time_point;
using std::chrono::steady_clock;
using std::chrono::seconds;
using std::chrono::milliseconds;
using namespace std;

typedef DeterministicSchedule DSched;

template <bool Dynamic = false, typename T>
void runElementTypeTest(T&& src) {
  MPMCQueue<T, std::atomic, Dynamic> cq(10);
  cq.blockingWrite(std::forward<T>(src));
  T dest;
  cq.blockingRead(dest);
  EXPECT_TRUE(cq.write(std::move(dest)));
  EXPECT_TRUE(cq.read(dest));
  auto soon1 = std::chrono::system_clock::now() + std::chrono::seconds(1);
  EXPECT_TRUE(cq.tryWriteUntil(soon1, std::move(dest)));
  EXPECT_TRUE(cq.read(dest));
  auto soon2 = std::chrono::steady_clock::now() + std::chrono::seconds(1);
  EXPECT_TRUE(cq.tryWriteUntil(soon2, std::move(dest)));
  EXPECT_TRUE(cq.read(dest));
}

struct RefCounted {
  static FOLLY_TLS int active_instances;

  mutable std::atomic<int> rc;

  RefCounted() : rc(0) {
    ++active_instances;
  }

  ~RefCounted() {
    --active_instances;
  }
};
FOLLY_TLS int RefCounted::active_instances;

void intrusive_ptr_add_ref(RefCounted const* p) {
  p->rc++;
}

void intrusive_ptr_release(RefCounted const* p) {
  if (--(p->rc) == 0) {
    delete p;
  }
}

TEST(MPMCQueue, lots_of_element_types) {
  runElementTypeTest(10);
  runElementTypeTest(string("abc"));
  runElementTypeTest(std::make_pair(10, string("def")));
  runElementTypeTest(vector<string>{{"abc"}});
  runElementTypeTest(std::make_shared<char>('a'));
  runElementTypeTest(folly::make_unique<char>('a'));
  runElementTypeTest(boost::intrusive_ptr<RefCounted>(new RefCounted));
  EXPECT_EQ(RefCounted::active_instances, 0);
}

TEST(MPMCQueue, lots_of_element_types_dynamic) {
  runElementTypeTest<true>(10);
  runElementTypeTest<true>(string("abc"));
  runElementTypeTest<true>(std::make_pair(10, string("def")));
  runElementTypeTest<true>(vector<string>{{"abc"}});
  runElementTypeTest<true>(std::make_shared<char>('a'));
  runElementTypeTest<true>(folly::make_unique<char>('a'));
  runElementTypeTest<true>(boost::intrusive_ptr<RefCounted>(new RefCounted));
  EXPECT_EQ(RefCounted::active_instances, 0);
}

TEST(MPMCQueue, single_thread_enqdeq) {
  // Non-dynamic version only.
  // False positive for dynamic version. Capacity can be temporarily
  // higher than specified.
  MPMCQueue<int> cq(10);

  for (int pass = 0; pass < 10; ++pass) {
    for (int i = 0; i < 10; ++i) {
      EXPECT_TRUE(cq.write(i));
    }
    EXPECT_FALSE(cq.write(-1));
    EXPECT_FALSE(cq.isEmpty());
    EXPECT_EQ(cq.size(), 10);

    for (int i = 0; i < 5; ++i) {
      int dest = -1;
      EXPECT_TRUE(cq.read(dest));
      EXPECT_EQ(dest, i);
    }
    for (int i = 5; i < 10; ++i) {
      int dest = -1;
      cq.blockingRead(dest);
      EXPECT_EQ(dest, i);
    }
    int dest = -1;
    EXPECT_FALSE(cq.read(dest));
    EXPECT_EQ(dest, -1);

    EXPECT_TRUE(cq.isEmpty());
    EXPECT_EQ(cq.size(), 0);
  }
}

uint64_t nowMicro() {
  timeval tv;
  gettimeofday(&tv, 0);
  return static_cast<uint64_t>(tv.tv_sec) * 1000000 + tv.tv_usec;
}

TEST(MPMCQueue, tryenq_capacity_test) {
  // Non-dynamic version only.
  // False positive for dynamic version. Capacity can be temporarily
  // higher than specified.
  for (size_t cap = 1; cap < 100; ++cap) {
    MPMCQueue<int> cq(cap);
    for (size_t i = 0; i < cap; ++i) {
      EXPECT_TRUE(cq.write(i));
    }
    EXPECT_FALSE(cq.write(100));
  }
}

TEST(MPMCQueue, enq_capacity_test) {
  // Non-dynamic version only.
  // False positive for dynamic version. Capacity can be temporarily
  // higher than specified.
  for (auto cap : { 1, 100, 10000 }) {
    MPMCQueue<int> cq(cap);
    for (int i = 0; i < cap; ++i) {
      cq.blockingWrite(i);
    }
    int t = 0;
    int when;
    auto thr = std::thread([&]{
      cq.blockingWrite(100);
      when = t;
    });
    usleep(2000);
    t = 1;
    int dummy;
    cq.blockingRead(dummy);
    thr.join();
    EXPECT_EQ(when, 1);
  }
}

TEST(MPMCQueue,mcmp_race){

    int consumers = 3;
    int producers = 3;
    int uplimit = 100;
    MPMCQueue<int> _queue(10);

    vector<thread> cthds;
    atomic_int rv(0);
    bool read_mission_completed = false;
    atomic_int waited_consumers(3);
    FOR_EACH_RANGE(i,0,consumers){
        std::thread ithread([&](){
            while(true){
                int readin,tmp;
                _queue.blockingRead(readin);
                if(readin < 0) {
                    waited_consumers --;
                    return;
                }
                do{
                    tmp = readin - 1;
                    //cout << rv << ' ' << tmp << ' ' << readin << endl;
                }while(!rv.compare_exchange_strong(tmp,readin));
            }
        });
        cthds.push_back( std::forward<thread>(ithread) );
    }

    //test cpu consumption
    this_thread::sleep_for(std::chrono::seconds(10));

    vector<thread> pthds;
    atomic_int wv(1);
    FOR_EACH_RANGE(i,0,producers){
        std::thread ithread([&](){
            while(true){
                int tv = wv.fetch_add(1);
                if(tv <= uplimit){
                    _queue.blockingWrite(tv);
                }
                if(tv >= uplimit) {
                    return ;
                }
            }
        });
        pthds.push_back( std::forward<thread>(ithread) );
    }

    for(auto& ithds : pthds){
        ithds.join();
    }

    while(waited_consumers > 0){
        //cout << "reader still wait, consumers = "<< waited_consumers << endl;
        if(_queue.isEmpty())
            _queue.write(-1);
    }

    for(auto& ithds : cthds){
        ithds.join();
    }

    EXPECT_EQ(rv,uplimit);

}

int main(){
    int argc = 2;
    char* argv[] = { "mcmp_test","--gtest_filter=MPMCQueue.mcmp_race"};
    testing::InitGoogleTest(&argc,argv);
    RUN_ALL_TESTS();
    return 0;
}
