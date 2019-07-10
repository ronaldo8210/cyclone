/***********************************************************************
#   > File Name   : epoll_poller.hpp
#   > Author      : ronaldo
#   > Description : 
#   > Create Time : 2019-07-08 21:27:30
***********************************************************************/
#pragma once

#include <poller.hpp>

#include <vector>

struct epoll_event;

namespace cyclone {

class EpollPoller : public Poller {
 public:
  EpollPoller(EventLoop *loop);

  ~EpollPoller() override;

  Timestamp poll(int timeout_ms, ChannelList *active_channels) override;

  void update_channel(Channel *channel) override;

  void remove_channel(Channel *channel) override;

 private:
  typedef std::vector<epoll_event> EventList;

  void fill_active_channels(int num_active, ChannelList *active_channels) const;

  void update(int operation, Channel *channel);

  const int init_event_size_ = 32;

  int epoll_fd_;

  EventList events_;
};

}  // namespace cyclone
