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
class InetAddress;

class TcpServer {
 public:
  TcpServer(EventLoop *acceptor_loop, const InetAddress &listen_addr);

  ~TcpServer();

  void set_connect_callback(connect_callback &cb);

  void set_message_callback(message_callback &cb);

  void set_write_complete_callback(write_complete_callback &cb);

  void start();

  void set_io_thread_num(int io_thread_num);

  EventLoop* acceptor_loop();

 private:
  // connection实例必须用shared_ptr管理，因为要用到shared_from_this和weak_ptr
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
