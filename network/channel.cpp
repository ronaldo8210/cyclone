/***********************************************************************
#   > File Name   : channel.cpp
#   > Author      : ronaldo
#   > Description : 
#   > Create Time : 2019-04-15 13:41:02
***********************************************************************/

#include <assert.h>
#include <channel.hpp>
#include <event_loop.hpp>
#include <poll.h>

namespace cyclone {

// 先考虑读写事件，不考虑带外数据等    
const int Channel::kNoneEvent_ = 0;
const int Channel::kReadEvent_ = POLLIN;
const int Channel::kWriteEvent_ = POLLOUT;

int Channel::st_new_ = 0;
int Channel::st_add_loop_ = 1;
int Channel::st_delete_ = -1;

Channel::Channel(EventLoop *io_loop, int fd) 
    : io_loop_(io_loop), 
    fd_(fd), 
    events_(0),
    revents_(0),
    state_(st_new_),
    tied_(false),
    added_to_io_loop_(false),
    event_handling_(false) {

}

Channel::~Channel() {
  assert(!event_handling_);
  assert(!added_to_io_loop_);
}

void Channel::tie(const std::shared_ptr<void>& ptr) {
  tied_obj_ptr_ = ptr;
  tied_ = true;
}

void Channel::handle_event(Timestamp recv_time) {
  if (tied_ && tied_obj_ptr_.lock()) {
    event_handling_ = true;

    // 需要考虑tcp连接上的各种网络异常情况
    
    if (revents_ & POLLIN) {
      if (read_cb_) {
        read_cb_(recv_time);
      }
    }

    if (revents_ & POLLOUT) {
      if (write_cb_) {
        write_cb_()  ;  
      }
    }

    event_handling_ = false;
  }
}

void Channel::remove() {
  assert(is_none_event());
  added_to_io_loop_ = false;
  io_loop_->remove_channel(this);
}

void Channel::update() {
  added_to_io_loop_ = true;
  io_loop_->update_channel(this);
}

}  // namespace cyclone
