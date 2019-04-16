/***********************************************************************
#   > File Name   : src/event_loop.cpp
#   > Author      : ronaldo
#   > Description : 
#   > Create Time : 2019-04-15 18:56:51
***********************************************************************/

#include <event_loop.hpp>

#include <iostream>

using namespace std;

namespace cyclone {

EventLoop::EventLoop() {

}

EventLoop::~EventLoop() {

}

void EventLoop::loop() {
  looping_ = true;
  cout << "work starting..." << endl;
}

}  // namespace cyclone
