/***********************************************************************
#   > File Name   : connection.hpp
#   > Author      : ronaldo
#   > Description : 
#   > Create Time : 2019-04-15 22:59:14
***********************************************************************/
#pragma once

#include <callbacks.hpp>

namespace cyclone {

namespace io {

class EventLoop;

class Connection : boost::noncopyable {
 public:
  Connection();
  ~Connection();

 private:
  enum phase { kConnecting, kConnected, kDisConnecting, kDisconnected }

  void send_in_io_loop(string message);
  void shutdown_in_io_loop();

  EventLoop *io_loop;
  Buffer input_buffer;
  Buffer output_buffer;
  phase phase_;
};

}  // namespace io

}  // namespace cyclone
