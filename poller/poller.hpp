/***********************************************************************
#   > File Name   : poller.hpp
#   > Author      : ronaldo
#   > Description : 
#   > Create Time : 2019-07-08 21:00:16
***********************************************************************/
#pragma once

#include <vector>
#include <map>
#include <timestamp.hpp>

namespace cyclone {

class Channel;
class EventLoop;

class Poller {
 public:
  typedef std::vector<Channel*> ChannelList;

  Poller(EventLoop *event_loop);
  
  virtual ~Poller();

  virtual Timestamp poll(int timeout_ms, ChannelList *channel_list) = 0;

  // 更改一个fd对应的channel的读写关注状态
  virtual void update_channel(Channel *channel) = 0;

  virtual void remove_channel(Channel *channel) = 0;

  virtual bool has_channel(Channel *channel) const;

  static Poller* get_poller(EventLoop *event_loop);
  
 protected:
  // Poller类实例是io_loop内所有channel的实际管理者
  typedef std::map<int, Channel*> ChannelMap;

  ChannelMap channels_;

  EventLoop* owner_loop_;
};

}  // namespace cyclone
