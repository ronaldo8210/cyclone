/***********************************************************************
#   > File Name   : include/tcp_server.hpp
#   > Author      : ronaldo
#   > Description : 
#   > Create Time : 2019-05-09 16:15:52
***********************************************************************/
#pragma once

#include <callbacks.hpp>
#include <map>
#include <memory>
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

  void set_io_thread_num(int io_thread_num);

  EventLoop* acceptor_loop() { return acceptor_loop_; }

 private:
  // connection实例必须用shared_ptr管理，因为要用到shared_from_this和weak_ptr
  typedef map<string, connection_ptr> connection_map;

  void create_connection(int connfd, const InetAddress &peer_addr);

  void remove_connection(const connection_ptr &conn);

  void remove_connection_in_loop(const connection_ptr &conn);

  // 被赋值为用户在自定义Server中设置的回调函数
  // TcpServer对象被包含在用户自定义Server中，用户自定义Server对象一般是main函数的栈上对象，生命周期是随进程一直存在的
  // 所以用bind的时候传入用户自定义Server对象的this指针即可，不用担心用户自定义Server对象被意外析构
  // 这些回调被所有io线程调用，因为回调函数是无状态的，且参数是各个Connection实例的引用，所以线程安全
  connect_callback connect_cb_;

  message_callback message_cb_;

  write_complete_callback write_complete_cb_;

  EventLoop *acceptor_loop_;

  shared_ptr<EventLoopThreadPool> io_thread_pool_;

  unique_ptr<Acceptor> acceptor_;

  int next_conn_id_;

  // 在这里集中管理所有Connection实例，每个Connection用shared_ptr管理
  connection_map connections_;

  bool started_;

  int io_thread_num_;
};

}  // namespace cyclone
