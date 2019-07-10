/***********************************************************************
#   > File Name   : include/channel.hpp
#   > Author      : ronaldo
#   > Description : 
#   > Create Time : 2019-04-15 12:20:10
***********************************************************************/

#include <boost/function.hpp>

namespace cyclone {

class EventLoop;

class Channel {
 public:
  typedef boost::function<void()> event_callback;

  void set_read_cb(event_callback& cb) { read_cb_ = cb; }
  void set_write_cb(event_callback& cb) { write_cb_ = cb; }
  void set_error_cb(event_callback& cb) { error_cb_ = cb; }

  void handle_event();

 private:
  static const int kNoneEvent;
  static const int kReadEvent;
  static const int kWriteEvent;

  EventLoop* eventLoop;
  int fd;

  event_callback read_cb_;
  event_callback write_cb_;
  event_callback error_cb_;
};

}  // namespace cyclone
