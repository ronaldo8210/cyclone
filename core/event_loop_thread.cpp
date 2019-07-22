/***********************************************************************
#   > File Name   : event_loop_thread.cpp
#   > Author      : ronaldo
#   > Description : 
#   > Create Time : 2019-07-22 18:28:20
***********************************************************************/

#include <event_loop_thread.hpp>

#include <event_loop.hpp>

namespace cyclone {

EventLoopThread::EventLoopThread() 
    : loop_(nullptr), 
    exit_(false), 
    thread_(std::bind(&EventLoopThread::thread_func, this)) {

}

EventLoopThread::~EventLoopThread() {
  exit_ = true;
  if (loop_) {
    loop_->quit();
    thread_.join();
  }
}

void EventLoopThread::thread_func() {
  EventLoop loop;
  loop_ = &loop;
  loop.loop();
  loop_ = nullptr; 
}

}  // namespace cyclone
