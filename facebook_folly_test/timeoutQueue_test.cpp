//##################################################################################
//###@author xudi                                                                  #
//###@Mail   xudi1989@ruc.edu.cn                                                   #
//###@Lisence GPL                                                                  #
//##################################################################################

#include <folly/TimeoutQueue.h>
#include <folly/portability/GTest.h>

using namespace folly;

TEST(TimeoutQueue, Simple) {
  typedef std::vector<TimeoutQueue::Id> EventVec;
  EventVec events;

  TimeoutQueue q;
  TimeoutQueue::Callback cb = [&events](
      TimeoutQueue::Id id, int64_t /* now */) { events.push_back(id); };

  EXPECT_EQ(1, q.add(0, 10, cb));
  EXPECT_EQ(2, q.add(0, 11, cb));
  EXPECT_EQ(3, q.addRepeating(0, 9, cb));

  EXPECT_TRUE(events.empty());
  EXPECT_EQ(21, q.runOnce(12));  // now+9

  bool r = (EventVec{3,1,2} == events);
  EXPECT_TRUE(r);

  events.clear();
  EXPECT_EQ(49, q.runOnce(40));
  r = (EventVec{3} == events);
  EXPECT_TRUE(r);
}

TEST(TimeoutQueue, Erase) {
  typedef std::vector<TimeoutQueue::Id> EventVec;
  EventVec events;

  TimeoutQueue q;
  TimeoutQueue::Callback cb =
      [&events, &q](TimeoutQueue::Id id, int64_t /* now */) {
        events.push_back(id);
        if (id == 2) {
          q.erase(1);
        }
      };

  EXPECT_EQ(1, q.addRepeating(0, 10, cb));
  EXPECT_EQ(2, q.add(0, 35, cb));

  int64_t now = 0;
  while (now < std::numeric_limits<int64_t>::max()) {
    now = q.runOnce(now);
  }

  bool r = (EventVec{1,1,1,2} == events);
  EXPECT_TRUE(r);
}

TEST(TimeoutQueue, RunOnceRepeating) {
  int count = 0;
  TimeoutQueue q;
  TimeoutQueue::Callback cb =
      [&count, &q](TimeoutQueue::Id id, int64_t /* now */) {
        if (++count == 100) {
          EXPECT_TRUE(q.erase(id));
        }
      };

  EXPECT_EQ(1, q.addRepeating(0, 0, cb));

  EXPECT_EQ(0, q.runOnce(0));
  EXPECT_EQ(1, count);
  EXPECT_EQ(0, q.runOnce(0));
  EXPECT_EQ(2, count);
  EXPECT_EQ(std::numeric_limits<int64_t>::max(), q.runLoop(0));
  EXPECT_EQ(100, count);
}

TEST(TimeoutQueue, RunOnceReschedule) {
  int count = 0;
  TimeoutQueue q;
  TimeoutQueue::Callback cb;
  cb = [&count, &q, &cb](TimeoutQueue::Id id, int64_t now) {
      if (++count < 100) {
        EXPECT_LT(id, q.add(now, 0, cb));
      }
    };

  EXPECT_EQ(1, q.add(0, 0, cb));

  EXPECT_EQ(0, q.runOnce(0));
  EXPECT_EQ(1, count);
  EXPECT_EQ(0, q.runOnce(0));
  EXPECT_EQ(2, count);
  EXPECT_EQ(std::numeric_limits<int64_t>::max(), q.runLoop(0));
  EXPECT_EQ(100, count);
}

int timeoutQueueTest(int argc,char* argv[]){
    testing::InitGoogleTest(&argc,argv);
    RUN_ALL_TESTS();
}
