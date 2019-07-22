/***********************************************************************
#   > File Name   : src/event_loop.cpp
#   > Author      : ronaldo
#   > Description : 
#   > Create Time : 2019-04-15 18:56:51
***********************************************************************/

#include <event_loop.hpp>

#include <channel.hpp>
#include <poller.hpp>
#include <assert.h>
#include <unistd.h>

namespace cyclone {

EventLoop::EventLoop() : 
    looping_(false), 
    quit_(false), 
    event_handling_(false), 
    pending_functors_handling_(false), 
    poller_(Poller::get_poller(this)),
    curr_active_channel_(nullptr),
    thread_id_(std::this_thread::get_id()) {
  if (get_loop_of_current_thread()) {
    // log error  
  } else {
    get_loop_of_current_thread() = this;  
  }
}

EventLoop::~EventLoop() {
  ::close(wakeup_fd_);
  get_loop_of_current_thread() = nullptr;
}

EventLoop* & EventLoop::get_loop_of_current_thread() {
  static thread_local EventLoop *loop = nullptr;
  return loop;
}

void EventLoop::loop() {
  assert_in_loop_thread();
  looping_ = true;
  
  while (!quit_) {
    active_channels_.clear();
    poll_return_ts_ = poller_->poll(poll_time_ms_, &active_channels_);

    event_handling_ = true;
    for (Channel *channel : active_channels_) {
      curr_active_channel_ = channel;
      channel->handle_event(poll_return_ts_);
    }
    event_handling_ = false;

    curr_active_channel_ = nullptr;
    exec_pending_functors();
  }

  looping_ = false;
}

void EventLoop::quit() {
  quit_ = true;
}

bool EventLoop::is_in_loop_thread() {
  return thread_id_ == std::this_thread::get_id();
}

void EventLoop::assert_in_loop_thread() {
  if (!is_in_loop_thread()) {
    abort();
  }
}

void EventLoop::run_in_loop(Functor functor) {
  if (is_in_loop_thread()) {
    functor();
  } else {
    // 如果是worker线程，则将可调用对象放入队列
    // 让io线程处理完一次事件循环后去调用functor，使用串行操作避免了线程间的互斥
    add_functor_queue(std::move(functor));  // 参数传递时触发Function的移动构造
  }
}

void EventLoop::add_functor_queue(Functor functor) {
  {
    std::lock_guard<std::mutex> lock(mutex_);
    pending_functors_.push_back(std::move(functor));
  }

  if (pending_functors_handling_) {
    // 队列中的functors已被swap出去并正在执行，如果后续一直没有io事件，则事件loop长时间不会返回
    // 为确保新加入队列的functors能及时被执行，需要令事件loop及时返回
    wakeup();  
  }
}

void EventLoop::exec_pending_functors() {
  std::vector<Functor> functors;

  {
    std::lock_guard<std::mutex> lock(mutex_);
    functors.swap(pending_functors_);
    pending_functors_handling_ = true;
  }

  for (const Functor &functor : functors) {
    functor();
  }

  pending_functors_handling_ = false;
}

void EventLoop::wakeup() {

}

void EventLoop::update_channel(Channel *channel) {
  // 所有channel的update操作都在io_loop线程中完成，串行操作，不需要加锁
  assert(channel->io_loop() == this);
  assert_in_loop_thread();
  poller_->update_channel(channel);
}

void EventLoop::remove_channel(Channel *channel) {
  // 不需要加锁
  assert(channel->io_loop() == this);
  assert_in_loop_thread();
  poller_->remove_channel(channel);
}

void EventLoop::has_channel(Channel *channel) {
  // 不需要加锁
  assert(channel->io_loop() == this);
  assert_in_loop_thread();
  poller_->has_channel(channel);
}

}  // namespace cyclone
