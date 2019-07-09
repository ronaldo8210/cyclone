/***********************************************************************
#   > File Name   : poller.cpp
#   > Author      : ronaldo
#   > Description : 
#   > Create Time : 2019-07-08 21:17:48
***********************************************************************/

#include <poller.hpp>

#include <epoll_poller.hpp>

namespace cyclone {

Poller::Poller(EventLoop *event_loop) : owner_loop_(event_loop) {

}

Poller::~Poller() = default;

Poller::get_poller(EventLoop *event_loop) {
  // 先只支持epoll模式
  return new EpollPoller(event_loop);
}

}  // namespace cyclone
