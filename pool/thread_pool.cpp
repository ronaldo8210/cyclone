/***********************************************************************
#   > File Name   : thread_pool.cpp
#   > Author      : ronaldo
#   > Description : 
#   > Create Time : 2019-07-22 16:23:24
***********************************************************************/

#include <thread_pool.hpp>

#include <assert.h>

namespace cyclone {

ThreadPool::ThreadPool(const string& name) 
    : max_queue_size_(0), 
    running_(false) {

}

ThreadPool::~ThreadPool() {
  if (running_) {
    stop(); 
  }
}
  
size_t ThreadPool::queue_size() {
  unique_lock<mutex> lock(mtx_);
  return queue_.size();
}

void ThreadPool::start(int num_threads) {
  assert(threads_.empty());

  for (unsigned int i = 0; i < num_threads; ++i) {
    threads_.push_back(thread(&ThreadPool::run, this));
  }

  running_ = true;
}

void ThreadPool::stop() {
  {
    lock_guard<mutex> lock(mtx_);
    running_ = false;
    // 唤醒等待队列中任务的线程，让它们退出循环
    cv_not_empty_.notify_all();
  }

  for (thread &thrd : threads_) {
    thrd.join(); 
  }
}

void ThreadPool::post(Task func) {
  unique_lock<mutex> lock(mtx_);

  // 如果任务队列已满，不能直接压入任务，要等到worker线程取走任务留出空间后才能压入
  while (is_full()) {
    cv_not_full_.wait(lock);
  }

  queue_.push_back(std::move(func));
  // 压入了一个任务，只唤醒一个等待任务的worker线程
  cv_not_empty_.notify_one();
}

bool ThreadPool::is_full() const {
  // 不可被直接调用  
  return (queue_.size() >= max_queue_size_);
}

void ThreadPool::run() {
  while (running_) {
    Task task(take());
    if (task) {
      task();
    }
  }
}

ThreadPool::Task ThreadPool::take() {
  unique_lock<mutex> lock(mtx_);
  while (running_ && queue_.empty()) {
    cv_not_empty_.wait(lock);
  }

  Task task = queue_.front();
  queue_.pop_front();
  cv_not_full_.notify_one();

  return task;
}

}  // namespace cyclone
