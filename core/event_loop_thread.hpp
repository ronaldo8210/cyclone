/***********************************************************************
#   > File Name   : event_loop_thread.hpp
#   > Author      : ronaldo
#   > Description : 
#   > Create Time : 2019-07-22 17:44:31
***********************************************************************/
#pragma once

#include <nocopyable.hpp>
#include <thread>

namespace cyclone {

class EventLoop;

// io_loop线程的简单封装
class EventLoopThread : public nocopyable {
 public:
  EventLoopThread();

  ~EventLoopThread();

  EventLoop* get_loop() { return loop_; }

 private:
  void thread_func();

  EventLoop *loop_;

  std::thread thread_;

  bool exit_;
};

}  // namespace cyclone
