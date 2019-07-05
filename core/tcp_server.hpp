/***********************************************************************
#   > File Name   : include/tcp_server.hpp
#   > Author      : ronaldo
#   > Description : 
#   > Create Time : 2019-05-09 16:15:52
***********************************************************************/
#pragma once

#include <callbacks.hpp>
#include <map>
#include <string>

using namespace std;

namespace cyclone {

class Acceptor;
class EventLoop;
class EventLoopThreadPool;

class TcpServer {
 public:
  TcpServer();
  ~TcpServer();

  void set_connect_callback(connect_callback &cb) {
    connect_cb_ = cb;
  }

  void set_message_callback(message_callback &cb) {
    message_cb_ = cb;
  }
  
  void set_write_complete_callback(write_complete_callback &cb) {
    write_complete_cb_ = cb;
  }

  void start();

 private:
  typedef map<string, connection_ptr> connection_map;

  void create_connection(int conn_fd);
  void remove_connection();

  connect_callback connect_cb_;
  message_callback message_cb_;
  write_complete_callback write_complete_cb_;

  EventLoop *acceptor_loop_;
  EventLoopThreadPool *io_thread_pool_;
  Acceptor *acceptor_;
  connection_map connections_;
  bool started_;
  int io_thread_num_;
  int next_conn_id_;
};

}  // namespace cyclone
