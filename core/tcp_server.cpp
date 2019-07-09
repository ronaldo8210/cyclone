/***********************************************************************
#   > File Name   : tcp_server.cpp
#   > Author      : ronaldo
#   > Description : 
#   > Create Time : 2019-04-15 22:57:55
***********************************************************************/

#include <tcp_server.hpp>

namespace cyclone {

TcpServer::TcpServer(EventLoop *acceptor_loop, const InetAddress &listen_addr) {

}

TcpServer::~TcpServer() {

}

void TcpServer::set_io_thread_num(int io_thread_num) {
  io_thread_num_ = io_thread_num;  
}

void TcpServer::start() {

}

// must be thread-safe
void TcpServer::create_connection(int connfd) {

}

// must be thread-safe
void TcpServer::remove_connection() {

}

void TcpServer::set_connect_callback(connect_callback &cb) {
  connect_cb_ = cb;
}

void TcpServer::set_message_callback(message_callback &cb) {
  message_cb_ = cb;
}
  
void TcpServer::set_write_complete_callback(write_complete_callback &cb) {
  write_complete_cb_ = cb;
}

}  // namespace cyclone
