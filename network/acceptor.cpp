/***********************************************************************
#   > File Name   : acceptor.cpp
#   > Author      : ronaldo
#   > Description : 
#   > Create Time : 2019-07-22 20:47:43
***********************************************************************/

#include <acceptor.hpp>
#include <event_loop.hpp>
#include <inet_address.hpp>
#include <sock_ops.hpp>

namespace cyclone {

Acceptor::Acceptor(EventLoop *base_loop, const InetAddress &listen_addr, bool reuse_port)
    : base_loop_(base_loop), 
    accept_sock_(sockops::create_nonblocking(listen_addr.family())), 
    accept_channel_(base_loop_, accept_sock_.fd()) {
  accept_sock_.bind(listen_addr);
  accept_channel_.set_read_cb(bind(&Acceptor::handle_read, this));
}

Acceptor::~Acceptor() {
  accept_channel_.disable_all();
  accept_channel_.remove();
}

void Acceptor::listen() {
  base_loop_->assert_in_loop_thread();

  accept_sock_.listen();
  accept_channel_.enable_reading();
}
  
void Acceptor::handle_read() {
  base_loop_->assert_in_loop_thread();

  InetAddress peer_addr;
  int connfd = accept_sock_.accept(&peer_addr);
  if (connfd > 0) {
    // 转到tcp_server的new_connection处理函数，新建connection对象并集中管理
    new_connection_cb_(connfd, peer_addr);
  } else {
    // 网络错误处理    
  }
}

}  // namespace cyclone
