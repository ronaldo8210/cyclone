/***********************************************************************
#   > File Name   : poller.hpp
#   > Author      : ronaldo
#   > Description : 
#   > Create Time : 2019-07-08 21:00:16
***********************************************************************/
#pragma once

#include <vector>
#include <map>

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

  static Poller* get_poller(EventLoop *event_loop);
  
 protected:
  typedef std::map<int, Channel*> ChannelMap;

  ChannelMap channels_;

  EventLoop* owner_loop_;
};

}  // namespace cyclone
