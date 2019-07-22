/***********************************************************************
#   > File Name   : include/channel.hpp
#   > Author      : ronaldo
#   > Description : 
#   > Create Time : 2019-04-15 12:20:10
***********************************************************************/

#include <memory>
#include <functional>
#include <nocopyable.hpp>
#include <timestamp.hpp>

using namespace std;

namespace cyclone {

class EventLoop;

class Channel : public nocopyable {
 public:
  typedef function<void()> event_callback;

  typedef function<void(Timestamp)> read_event_callback;

  Channel(EventLoop *io_loop, int fd);

  ~Channel();

  // fd描述符是否加入epoll的状态标志
  static int st_new_;

  static int st_add_loop_;

  static int st_delete_;

  void handle_event(Timestamp recv_time);

  void set_read_cb(read_event_callback cb) { read_cb_ = std::move(cb); }

  void set_write_cb(event_callback cb) { write_cb_ = std::move(cb); }

  void set_close_cb(event_callback cb) { close_cb_ = std::move(cb); }

  void set_error_cb(event_callback cb) { error_cb_ = std::move(cb); }

  // 绑定与其对应的Connection实例
  void tie(const std::shared_ptr<void>&);

  int fd() const { return fd_; }

  int events() const { return events_; }

  void set_revents(int revt) { revents_ = revt; }

  void enable_reading() {
    events_ |= kReadEvent_;
    update();
  }

  void disable_reading() {
    events_ &= ~kReadEvent_;  
    update();
  }

  void enable_writing() {
    events_ |= kWriteEvent_;
    update();
  }

  void disable_writing() {
    events_ &= ~kWriteEvent_;
    update();
  }

  void disable_all() {
    events_ = kNoneEvent_;
    update();
  }

  bool is_reading() const {
    return events_ & kReadEvent_;  
  }

  bool is_writing() const {
    return events_ & kWriteEvent_;  
  }

  bool is_none_event() const {
    return events_ == kNoneEvent_;  
  }

  int state() { return state_; }

  void set_state(int state) { state_ = state; }

  EventLoop* io_loop() { return io_loop_; }

  void remove();

 private:

  void update();

  static const int kNoneEvent_;

  static const int kReadEvent_;

  static const int kWriteEvent_;

  EventLoop *io_loop_;

  // fd描述符是否加入epoll的状态标志
  int state_;

  // channel本身不拥有fd，不负责fd的关闭等操作
  const int fd_;

  bool event_handling_;

  bool added_to_io_loop_;

  bool tied_;

  // 与对应的Connection实例关联上，不增加引用计数，所以用weak_ptr
  std::weak_ptr<void> tied_obj_ptr_;

  // fd描述符关注的事件
  int events_;

  // epoll返回时fd上的触发事件
  int revents_;

  read_event_callback read_cb_;

  event_callback write_cb_;

  event_callback close_cb_;

  event_callback error_cb_;
};

}  // namespace cyclone
