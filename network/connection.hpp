/***********************************************************************
#   > File Name   : connection.hpp
#   > Author      : ronaldo
#   > Description : 
#   > Create Time : 2019-04-15 22:59:14
***********************************************************************/
#pragma once

#include <buffer.hpp>
#include <callbacks.hpp>
#include <inet_address.hpp>
#include <nocopyable.hpp>
#include <timestamp.hpp>

#include <memory>

using namespace std;

namespace cyclone {

using namespace std;

class EventLoop;
class Socket;
class Channel;

class Connection : public nocopyable, public enable_shared_from_this<Connection> {
 public:
  Connection(EventLoop *io_loop, int fd, const InetAddress &local_addr, 
          const InetAddress &peer_addr);

  ~Connection();
  
  EventLoop* io_loop() { return io_loop_; }

  bool connected() const { return state_ == kConnected; }

  bool disconnected() const { return state_ == kDisconnected; }

  void set_connect_callback(connect_callback &connect_cb) {
    connect_cb_ = connect_cb;   
  }

  void set_message_callback(message_callback &message_cb) {
    message_cb_ = message_cb;
  }
  
  void set_write_complete_callback(write_complete_callback &write_complete_cb) {
    write_complete_cb_ = write_complete_cb;
  }

  void set_close_callback(close_callback &close_cb) {
    close_cb_ = close_cb;
  }

  void set_highwater_mark_callback(highwater_mark_callback &highwater_mark_cb, size_t highwater_mark) {
    highwater_mark_cb_ = highwater_mark_cb;
    highwater_mark_ = highwater_mark;
  }

  // 通常在worker线程池中被调用 
  // 为避免buffer的读写竞态, 内部会调用send_in_io_loop() 将实际的写操作放到io_loop的task队列中
  void send(const char *message, size_t len);

  void send(Buffer *buffer);

  // 只关闭fd描述符的写端，仍然可读
  void shutdown();

  void force_close();

  void connection_established();

  void connection_destroyed();

 private:
  enum state_e { kConnecting, kConnected, kDisConnecting, kDisconnected };

  void handle_read(Timestamp recv_time);

  void handle_write();

  void handle_close();

  void handle_error();

  void send_in_io_loop(const char *data, size_t len);

  void shutdown_in_io_loop();

  void set_state(state_e st) { state_ = st; }

  connect_callback connect_cb_;

  // 从fd读到数据后，执行的回调，一般是用户在自定义server中设置的回调
  message_callback message_cb_;

  write_complete_callback write_complete_cb_;

  close_callback close_cb_; 

  highwater_mark_callback highwater_mark_cb_;

  EventLoop *io_loop_;

  // 连接断开，Connection对象析构时，控制Socket对象的析构
  unique_ptr<Socket> socket_;

  // 连接断开，Connection对象析构时，控制Channel对象的析构
  unique_ptr<Channel> channel_;

  const InetAddress local_addr_;

  const InetAddress peer_addr_;

  Buffer recv_buffer_;

  Buffer send_buffer_;

  size_t highwater_mark_;

  state_e state_;
};

}  // namespace cyclone
