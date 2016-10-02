//##################################################################################
//###@author xudi                                                                  #
//###@Mail   xudi1989@ruc.edu.cn                                                   #
//###@Lisence GPL                                                                  #
//##################################################################################
/*
 * Copyright 2016 Facebook, Inc.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <folly/Subprocess.h>
#include <folly/Exception.h>
#include <folly/Format.h>
#include <folly/FileUtil.h>
#include <folly/String.h>
#include <folly/gen/String.h>
#include <folly/gen/File.h>
#include <folly/portability/Unistd.h>
#include <folly/gen/Core.h>
#include <folly/File.h>

#include <iostream>
#include <string>
#include <functional>
#include <vector>

#include <gtest/gtest.h>
#include <boost/filesystem.hpp>

using namespace folly;
using namespace std;

TEST(PopenSubprocessTest, PopenRead) {
  Subprocess proc("ls /home/xudi/", Subprocess::pipeStdout());
  int found = 0;
  folly::gen::byLine(File(proc.stdout())) |
    [&] (StringPiece line) {
      if (line == "github" || line == "cpp" || line == "workspace") {
        ++found;
      }
      cout << line << endl;
    };
  EXPECT_EQ(3, found);
  proc.waitChecked();
}


TEST(CommunicateSubprocessTest, Read) {
  Subprocess proc("/bin/echo -n foo bar",
                  Subprocess::pipeStdout());
  auto p = proc.communicate();
  EXPECT_EQ("foo bar", p.first);
  proc.waitChecked();
}

TEST(CommunicateSubprocessTest, Write) {
  const int numLines = 1 << 10;
  std::string line("hello\n");
  std::string data;
  data.reserve(numLines * line.size());
  for (int i = 0; i < numLines; ++i) {
    data.append(line);
  }

  Subprocess proc("wc -l", Subprocess::pipeStdin() | Subprocess::pipeStdout());
  auto p = proc.communicate(data);
  EXPECT_EQ(folly::format("{}\n", numLines).str(), p.first);
  proc.waitChecked();
}

bool readToString(int fd, std::string& buf, size_t maxSize) {
  buf.resize(maxSize);
  char* dest = &buf.front();
  size_t remaining = maxSize;

  ssize_t n = -1;
  while (remaining) {
    n = read(fd, dest, remaining);
    if (n == -1) {
      if (errno == EINTR) {
        continue;
      }
      if (errno == EAGAIN) {
        break;
      }
      cout << "[Failed] Reading!" << endl;
    } else if (n == 0) {
      break;
    }
    dest += n;
    remaining -= n;
  }
  buf.resize(dest - buf.data());
  return (n == 0);
}

TEST(SUBPROCESS_CALL_BACK, READ_WRITE){
    string cmd = "/bin/ps aux | grep codeblocks";
    Subprocess proc(cmd,Subprocess::pipeStdin() | Subprocess::pipeStdout());
    proc.communicate(
        Subprocess::readLinesCallback(
          [](int fd, folly::StringPiece s) {
            std::cout << fd << " said: " << s;
            return false;
          }
        ),
        [](int pdf, int cfd){ return true; }  // Don't write to the child
      );
    proc.waitChecked();
}


TEST(SUBPROCESS_CALL_BACK,CREATE_NEW_FILE){
    string fname = "/home/xudi/tmp/fmap_test";
    if(boost::filesystem::exists(fname)){
        boost::filesystem::remove(fname);
    }
    folly::Subprocess proc2("/bin/echo hello >> /home/xudi/tmp/fmap_test");
    proc2.waitChecked();
}


int subProcessTest(int argc,char* argv[]){
    testing::InitGoogleTest(&argc,argv);
    RUN_ALL_TESTS();
    return 0;
}

