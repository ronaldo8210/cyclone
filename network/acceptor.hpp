/***********************************************************************
#   > File Name   : acceptor.hpp
#   > Author      : ronaldo
#   > Description : 
#   > Create Time : 2019-07-22 20:47:37
***********************************************************************/
#pragma once

#include <channel.hpp>
#include <functional>
#include <nocopyable.hpp>
#include <socket.hpp>

using namespace std;

namespace cyclone {

class EventLoop;
class InetAddress;

class Acceptor {
 public:
  typedef function<void(int sock_fd, const InetAddress& peer_addr)> new_connection_callback;

  Acceptor(EventLoop *base_loop, const InetAddress &listen_addr, bool reuse_port = false);

  ~Acceptor();

  void set_new_connection_cb(const new_connection_callback &cb) {
    new_connection_cb_ = cb;
  }

  void listen();
  
 private:
  void handle_read();

  EventLoop *base_loop_;

  Socket accept_sock_;

  Channel accept_channel_;

  new_connection_callback new_connection_cb_;
};

}  // namespace cyclone
