/***********************************************************************
#   > File Name   : epoll_poller.cpp
#   > Author      : ronaldo
#   > Description : 
#   > Create Time : 2019-07-08 21:27:35
***********************************************************************/

#include <epoll_poller.hpp>

#include <channel.hpp>
#include <assert.h>
#include <string.h>
#include <sys/epoll.h>
#include <unistd.h>

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
  // 在io_loop线程中被调用，不需要加锁
  int fd = channel->fd();
  const int state = channel->state();

  if (state == Channel::st_new_) {
    assert(channels_.find(fd) == channels_.end());
    channels_[fd] = channel;
    update(EPOLL_CTL_ADD, channel);
    // 已经加入epoll，更改状态
    channel->set_state(Channel::st_add_loop_);
  } else if (state == Channel::st_add_loop_) {
    assert(channels_.find(fd) != channels_.end());
    assert(channels_[fd] == channel);
    if (channel->is_none_event()) {
      // fd上不关注读写事件了，从epoll中删除该fd
      update(EPOLL_CTL_DEL, channel);
      channel->set_state(Channel::st_delete_);
    } else {
      update(EPOLL_CTL_MOD, channel);
    }
  } else if (state == Channel::st_delete_) {

  } else {
    // 错误，未知状态
  }
}

void EpollPoller::remove_channel(Channel *channel) {
  // 不需要加锁
  int fd = channel->fd();
  int state = channel->state();

  assert(channel->is_none_event());
  assert(channels_.find(fd) != channels_.end());
  assert(channels_[fd] == channel);

  size_t n = channels_.erase(fd);
  update(EPOLL_CTL_DEL, channel);
}

void EpollPoller::fill_active_channels(int num_active, ChannelList *active_channels) const {
  for (int idx = 0; idx < num_active; ++idx) {
    Channel *channel = static_cast<Channel*>(events_[idx].data.ptr);
    channel->set_revents(events_[idx].events);
    active_channels->push_back(channel);
  }
}

void EpollPoller::update(int operation, Channel *channel) {
  struct epoll_event event;
  memset(&event, 0, sizeof(event));
  event.events = channel->events();
  event.data.ptr = channel;
  int fd = channel->fd();

  if (::epoll_ctl(epoll_fd_, operation, fd, &event) < 0) {
    // error log  
  }
}

}  // namespace cyclone
