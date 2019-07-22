/***********************************************************************
#   > File Name   : event_loop_thread_pool.hpp
#   > Author      : ronaldo
#   > Description : 
#   > Create Time : 2019-07-22 19:35:11
***********************************************************************/
#pragma once

#include <memory>
#include <vector>
#include <nocopyable.hpp>

using namespace std;

namespace cyclone {

class EventLoop;
class EventLoopThread;

class EventLoopThreadPool {
 public:
  EventLoopThreadPool(EventLoop *base_loop);

  ~EventLoopThreadPool();

  void set_io_threads_num(int num) { io_threads_num_ = num; }

  EventLoop* get_next_io_loop();

  void start();

  bool started() const { return started_; }

 private:
  // acceptor所在的线程   
  EventLoop *base_loop_;

  int io_threads_num_;

  int next_io_loop_;

  vector<unique_ptr<EventLoopThread>> io_threads_;

  vector<EventLoop*> io_loops_;

  bool started_;
};

}  // namespace cyclone
