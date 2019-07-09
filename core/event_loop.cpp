/***********************************************************************
#   > File Name   : src/event_loop.cpp
#   > Author      : ronaldo
#   > Description : 
#   > Create Time : 2019-04-15 18:56:51
***********************************************************************/

#include <event_loop.hpp>

namespace cyclone {

    
EventLoop::EventLoop() : looping_(false), 
                         quit_(false),
                         event_handling_(false),
                         pending_functors_handling_(false),
                         poll_time_ms_(100) {

}

EventLoop::~EventLoop() {

}

EventLoop::current_loop() {

}

void EventLoop::loop() {
  looping_ = true;
  
  while (!quit_) {
    poll_->pool(poll_time_ms_, &active_channels_);

    for (Channel *channel : active_channels_) {
      channel->handle_event();
    }

    exec_pending_functors();
  }
}

void EventLoop::stop() {
  stop_ = true;
}

void is_in_loop_thread() {

}


}  // namespace cyclone
