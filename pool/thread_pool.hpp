/***********************************************************************
#   > File Name   : ThreadPool.hpp
#   > Author      : ronaldo
#   > Description : 
#   > Create Time : 2019-07-22 15:48:40
***********************************************************************/
#pragma once 

#include <condition_variable> 
#include <deque>
#include <functional>
#include <mutex>
#include <string>
#include <thread>
#include <vector>

#include <nocopyable.hpp>

using namespace std;

namespace cyclone {

// 简单的worker线程池，暂不支持任务返回结果，暂不支持work steal
class ThreadPool : public nocopyable {
 public:
  typedef function<void()> Task;

  explicit ThreadPool(const string& name = "thread_pool");

  ~ThreadPool();

  void set_max_queue_size(size_t max_size) {
    max_queue_size_ = max_size;
  }
  
  size_t queue_size();

  void start(int num_threads);

  void stop();

  void post(Task func);

 private:
  bool is_full() const;

  void run();

  Task take();

  mutex mtx_;

  condition_variable cv_not_empty_;

  condition_variable cv_not_full_;

  vector<thread> threads_;

  deque<Task> queue_;

  size_t max_queue_size_;

  bool running_;
};

}  // namespace cyclone
