/***********************************************************************
#   > File Name   : event_loop_thread_pool.cpp
#   > Author      : ronaldo
#   > Description : 
#   > Create Time : 2019-07-22 19:35:55
***********************************************************************/

#include <event_loop_thread_pool.hpp>

#include <event_loop_thread.hpp>

namespace cyclone {

EventLoopThreadPool::EventLoopThreadPool(EventLoop *base_loop) 
    : base_loop_(base_loop), 
    io_threads_num_(0), 
    next_io_loop_(0),
    started_(false) {

}

EventLoopThreadPool::~EventLoopThreadPool() {
  // io_loops_存储的是栈上对象的指针，这里不需要释放 
}

EventLoop* EventLoopThreadPool::get_next_io_loop() {
  if (!io_loops_.empty()) {
    // 轮询
    EventLoop *io_loop = io_loops_[next_io_loop_++];
    if (static_cast<size_t>(next_io_loop_) >= io_loops_.size()) {
      next_io_loop_ = 0;
    }
    return io_loop;
  } else {
    return base_loop_;  
  }
}

void EventLoopThreadPool::start() {
  for (int i = 0; i < io_threads_num_; ++i) {
    EventLoopThread *io_thrd = new EventLoopThread();
    io_threads_.push_back(unique_ptr<EventLoopThread>(io_thrd));
    io_loops_.push_back(io_thrd->get_loop());
  }

  started_ = true;
}

}  // namespace cyclone
