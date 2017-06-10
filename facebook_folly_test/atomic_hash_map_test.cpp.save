
#include <folly/AtomicHashMap.h>

#include <glog/logging.h>
#include <thread>
#include <atomic>
#include <memory>

#include <folly/Assume.h>
#include <folly/Benchmark.h>
#include <folly/Conv.h>
#include <folly/portability/Atomic.h>
#include <folly/portability/GTest.h>
#include <folly/portability/SysTime.h>

using std::vector;
using std::string;
using folly::AtomicHashMap;
using folly::AtomicHashArray;
using folly::StringPiece;

static int64_t nowInUsec() {
  timeval tv;
  gettimeofday(&tv, 0);
  return int64_t(tv.tv_sec) * 1000 * 1000 + tv.tv_usec;
}

TEST(Ahm, BasicStrings) {
  typedef AtomicHashMap<int64_t,string> AHM;
  AHM myMap(1024);
  EXPECT_TRUE(myMap.begin() == myMap.end());

  for (int i = 0; i < 100; ++i) {
    auto ret = myMap.insert(make_pair(i, folly::to<string>(i)));
    EXPECT_TRUE(ret.second);
    EXPECT_EQ(ret.first->second,folly::to<string>(i));
  }
  for (int i = 0; i < 100; ++i) {
    EXPECT_EQ(myMap.find(i)->second, folly::to<string>(i));
  }

  auto ret = myMap.insert(std::make_pair(999, "A"));
  EXPECT_TRUE(ret.second);
  ret = myMap.insert(std::make_pair(999, "B"));
  EXPECT_FALSE(ret.second);
  EXPECT_EQ(myMap.find(999)->second, "A"); // shouldn't have overwritten
  myMap.find(999)->second = "B";
  myMap.find(999)->second = "C";
  EXPECT_EQ(myMap.find(999)->second, "C");
  EXPECT_EQ(myMap.find(999)->first, 999);
}

class Counters {
private:
  // Note: Unfortunately can't currently put a std::atomic<int64_t> in
  // the value in ahm since it doesn't support types that are both non-copy
  // and non-move constructible yet.
  AtomicHashMap<int64_t,int64_t> ahm;

public:
  explicit Counters(size_t numCounters) : ahm(numCounters) {}

  void increment(int64_t obj_id) {
    auto ret = ahm.insert(std::make_pair(obj_id, 1));
    if (!ret.second) {
      // obj_id already exists, increment count
      __sync_fetch_and_add(&ret.first->second, 1);
    }
  }

  int64_t getValue(int64_t obj_id) {
    auto ret = ahm.find(obj_id);
    return ret != ahm.end() ? ret->second : 0;
  }

  // export the counters without blocking increments
  string toString() {
    string ret = "{\n";
    ret.reserve(ahm.size() * 32);
    for (const auto& e : ahm) {
      ret += folly::to<string>(
        "  [", e.first, ":", e.second, "]\n");
    }
    ret += "}\n";
    return ret;
  }
};

// If you get an error "terminate called without an active exception", there
// might be too many threads getting created - decrease numKeys and/or mult.
TEST(Ahm, counter) {
  const int numKeys = 10;
  const int mult = 2;
  Counters c(numKeys);
  vector<int64_t> keys;
  FOR_EACH_RANGE(i, 1, numKeys) {
    keys.push_back(i);
  }
  vector<std::thread> threads;
  for (auto key : keys) {
    FOR_EACH_RANGE(i, 0, key * mult) {
      threads.push_back(std::thread([&, key] { c.increment(key); }));
    }
  }
  for (auto& t : threads) {
    t.join();
  }
  string str = c.toString();
  for (auto key : keys) {
    int val = key * mult;
    EXPECT_EQ(val, c.getValue(key));
    EXPECT_NE(string::npos, str.find(folly::to<string>("[",key,":",val,"]")));
  }
}

TEST(Ahm, erase_find_race) {
  const uint64_t limit = 10000;
  AtomicHashMap<uint64_t, uint64_t> map(limit + 10);
  std::atomic<uint64_t> key {1};

  // Invariant: all values are equal to their keys.
  // At any moment there is one or two consecutive keys in the map.

  std::thread write_thread([&]() {
    while (true) {
      uint64_t k = ++key;
      if (k > limit) {
        break;
      }
      map.insert(k + 1, k + 1);
      map.erase(k);
    }
  });

  std::thread read_thread([&]() {
    while (true) {
      uint64_t k = key.load();
      if (k > limit) {
        break;
      }

      auto it = map.find(k);
      if (it != map.end()) {
        ASSERT_EQ(k, it->second);
      }
    }
  });

  read_thread.join();
  write_thread.join();
}

int atm_test(int argc,char* argv[]){

    ::testing::InitGoogleTest(&argc,argv);
    return (RUN_ALL_TESTS());
}
