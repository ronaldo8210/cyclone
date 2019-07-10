/***********************************************************************
#   > File Name   : socket.cpp
#   > Author      : ronaldo
#   > Description : 
#   > Create Time : 2019-07-10 16:51:24
***********************************************************************/
#include <socket.hpp>

#include <inet_address.hpp>
#include <sock_ops.hpp>
#include <memory.h>
#include <netinet/in.h>
#include <netinet/tcp.h>

namespace cyclone {

Socket::~Socket() {
  sockops::close(sockfd_);
}

void Socket::bind(const InetAddress &local_addr) {
  sockops::bind(sockfd_, local_addr.get_sock_addr());
}

void Socket::listen() {
  sockops::listen(sockfd_);
}

int Socket::accept(InetAddress *peer_addr) {
  struct sockaddr_in6 addr;
  memset(&addr, 0, sizeof(addr));
  int connfd = sockops::accept(sockfd_, &addr);
  return connfd;
}

void Socket::shutdown() {
  sockops::shutdown(sockfd_);
}

void Socket::set_tcp_nodelay(bool on) {
  int optval = on ? 1 : 0;
  ::setsockopt(sockfd_, IPPROTO_TCP, TCP_NODELAY, &optval, static_cast<socklen_t>(sizeof optval));
}

void Socket::set_reuse_addr(bool on) {

}

void Socket::set_reuse_port(bool on) {

}

void Socket::set_keepalive(bool on) {
  
}

}
