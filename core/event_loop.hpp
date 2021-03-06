/***********************************************************************
#   > File Name   : include/event_loop.hpp
#   > Author      : ronaldo
#   > Description : 
#   > Create Time : 2019-04-13 21:38:27
***********************************************************************/
#pragma once

#include <atomic>
#include <functional>
#include <mutex>
#include <thread>
#include <vector>

#include <timestamp.hpp>

namespace cyclone {

class Channel;
class Poller;
class TimeQueue;

class EventLoop {
 public:
     
  typedef std::function<void()> Functor;

  EventLoop();

  ~EventLoop();

  void loop();

  void quit();

  // 判断当前线程是否为执行事件循环的线程
  bool is_in_loop_thread();

  void assert_in_loop_thread();

  // 在执行事件循环的线程中执行functor
  void run_in_loop(Functor functor);

  // 将一个可调用对象压入队列
  void add_functor_queue(Functor functor);

  void exec_pending_functors();

  void wakeup();

  void update_channel(Channel *channel);

  void remove_channel(Channel *channel);

  void has_channel(Channel *channel);

 private:
  std::thread::id thread_id_;  

  static EventLoop* & get_loop_of_current_thread();

  // event_loop不负责管理channel，仅是保存激活的channel的指针
  // 通过激活的channel去调用对应的connection实例中的回调
  typedef std::vector<Channel*> ChannelList;

  bool looping_;

  std::atomic<bool> quit_;

  // const pid_t thread_id_;

  Poller *poller_;

  TimeQueue *time_queue_;

  Channel *curr_active_channel_;

  ChannelList active_channels_;

  // worker线程压入的等待执行的可调用对象列表
  std::vector<Functor> pending_functors_;

  bool event_handling_;

  bool pending_functors_handling_;

  int wakeup_fd_;

  Channel *wakeup_channel_;

  // 用于保护pending_functors_队列的互斥量
  std::mutex mutex_;

  const int poll_time_ms_ = 1000;

  Timestamp poll_return_ts_;
};

}  // namespace cyclone
