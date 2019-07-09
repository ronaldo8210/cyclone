/***********************************************************************
#   > File Name   : epoll_poller.cpp
#   > Author      : ronaldo
#   > Description : 
#   > Create Time : 2019-07-08 21:27:35
***********************************************************************/

#include <epoll_poller.hpp>

#include <sys/epoll.h>

namespace cyclone {

EpollPoller::EpollPoller(EventLoop *loop) : 
    Poller(loop), 
    epoll_fd_(::epoll_create1(EPOLL_CLOEXEC)), 
    events_(init_event_size_)  {

}

EpollPoller::~EpollPoller() {
  ::close(epoll_fd_);
}

Timestamp EpollPoller::poll(int timeout_ms, ChannelList *active_channels) {
  int num_events = ::epoll_wait(epoll_fd_, &*events_.begin(), 
          static_cast<int>(events_.size()), timeout_ms);

  Timestamp ts = Timestamp::now();

  if (num_events > 0) {
    fill_active_channels(num_events, active_channels);
  } else if (num_events == 0) {
    // epoll_wait直到超时也没有事件返回
  } else {
    // 被信号中断？
  }
}

void EpollPoller::update_channel(Channel *channel) {

}

void EpollPoller::remove_channel(Channel *channel) {

}

void EpollPoller::fill_active_channels(int num_active, ChannelList *active_channels) {
  for (int idx = 0; idx < num_events; ++idx) {
    Channel *channel = static_cast<Channel*>(events_[idx].data.ptr);
    channel->set_revents(events_[idx].events);
    active_channels.push_back(channel);
  }
}

void EpollPoller::update(int operation, Channel *channel) {

}

}  // namespace cyclone
