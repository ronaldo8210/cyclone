/***********************************************************************
#   > File Name   : socket.hpp
#   > Author      : ronaldo
#   > Description : 
#   > Create Time : 2019-07-10 16:51:19
***********************************************************************/
#pragma once

namespace cyclone {

class InetAddress;

class Socket {
 public:
  explicit Socket(int sockfd) : sockfd_(sockfd) {

  }

  ~Socket();

  void bind(const InetAddress &local_addr);

  void listen();

  int accept(InetAddress *peer_addr);

  void shutdown();

  void set_tcp_nodelay(bool on);

  void set_reuse_addr(bool on);

  void set_reuse_port(bool on);

  void set_keepalive(bool on);

 private:

  const int sockfd_;     
};

}  // namespace cyclone
