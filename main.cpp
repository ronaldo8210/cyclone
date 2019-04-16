/***********************************************************************
#   > File Name   : server/server.cpp
#   > Author      : ronaldo
#   > Description : 
#   > Create Time : 2019-04-15 20:49:14
***********************************************************************/

#include <event_loop.hpp>

#include <iostream>
#include <boost/thread/thread.hpp>

using namespace cyclone;
using namespace std;

void func() {
  cout << "run in thread!!" << endl;
}

int main(int argc, char** argv) {
  EventLoop eventLoop;
  eventLoop.loop();

  boost::thread thrd(&func);
  thrd.join();

  return 0;
}
