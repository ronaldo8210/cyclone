/***********************************************************************
#   > File Name   : connection.hpp
#   > Author      : ronaldo
#   > Description : 
#   > Create Time : 2019-04-15 22:59:14
***********************************************************************/
#pragma once

#include <callbacks.hpp>

namespace cyclone {

using namespace std;

class EventLoop;
class Socket;
class Channel;

class Connection :  
    public enable_shared_from_this<Connection> {
 public:
  Connection(EventLoop *io_loop, int fd);
  ~Connection();

  /*
  EventLoop* io_loop() { return io_loop_; }
  bool connected() { return phase_ == kConnected; }
  bool disconnected() { return phase_ == kDisconnected; }

  void set_connect_callback (connect_callback connect_cb) {
    connect_cb_ = connect_cb;  
  }

  void set_message_callback (message_callback message_cb) {
    message_cb_ = message_cb;
  }
  
  void set_write_complete_callback (write_complete__callback write_complete_cb) {
    write_complete_cb_ = write_complete_cb;
  }

  // usually called in worker thread
  // for avoid race condition, it will call send_in_io_loop() to post task into io_loop task queue
  void send(string message);
  void send(string&& message);
  void shutdown();
  void force_close();

  void connection_established();
  void connection_destroyed();

 private:
  enum phase { kConnecting, kConnected, kDisConnecting, kDisconnected };

  void send_in_io_loop(string message);
  void shutdown_in_io_loop();

  connect_callback connect_cb_;
  message_callback message_cb_;
  write_complete_callback write_complete_cb_;

  EventLoop *io_loop_;
  scoped_ptr<Socket> socket_;
  scoped_ptr<Channel> channel_;
  Buffer recv_buffer_;
  Buffer send_buffer_;
  phase phase_; 
  */
};

}  // namespace cyclone
