/***********************************************************************
#   > File Name   : tcp_server.cpp
#   > Author      : ronaldo
#   > Description : 
#   > Create Time : 2019-04-15 22:57:55
***********************************************************************/

#include <tcp_server.hpp>

#include <acceptor.hpp>
#include <connection.hpp>
#include <event_loop.hpp>
#include <event_loop_thread_pool.hpp>
#include <string>

namespace cyclone {

TcpServer::TcpServer(EventLoop *acceptor_loop, const InetAddress &listen_addr) 
    : acceptor_loop_(acceptor_loop), 
    acceptor_(new Acceptor(acceptor_loop, listen_addr)), 
    io_thread_pool_(new EventLoopThreadPool(acceptor_loop)), 
    next_conn_id_(1) {
  acceptor_->set_new_connection_cb(
          bind(&TcpServer::create_connection, this, placeholders::_1, placeholders::_2));
}

TcpServer::~TcpServer() {
  for (auto &item : connections_) {
    connection_ptr conn(item.second);
    item.second.reset();
    conn->io_loop()->add_functor_queue(
            bind(&Connection::connection_destroyed, conn));
  }
}

void TcpServer::set_io_thread_num(int io_thread_num) {
  io_thread_pool_->set_io_threads_num(io_thread_num);
}

void TcpServer::start() {
  io_thread_pool_->start();
  acceptor_loop_->add_functor_queue(
          bind(&Acceptor::listen, acceptor_.get()));
}

void TcpServer::create_connection(int connfd, const InetAddress &peer_addr) {
  EventLoop *io_loop = io_thread_pool_->get_next_io_loop();
  
  string name = "";  // TODO
  ++next_conn_id_;
  //InetAddress local_addr(sockops::get_local_addr(connfd));
  InetAddress local_addr;
  connection_ptr conn(new Connection(io_loop, connfd, name, local_addr, peer_addr));
  connections_[name] = conn;

  conn->set_connect_callback(connect_cb_);
  conn->set_message_callback(message_cb_);
  conn->set_write_complete_callback(write_complete_cb_);
  //conn->set_close_callback(
  //        bind(&TcpServer::remove_connection, this, placeholders::_1));

  io_loop->add_functor_queue(
          bind(&Connection::connection_established, conn));
}

void TcpServer::remove_connection(const connection_ptr &conn) {
  acceptor_loop_->add_functor_queue(
          bind(&TcpServer::remove_connection_in_loop, this, conn));
}

void TcpServer::remove_connection_in_loop(const connection_ptr &conn) {
  size_t n = connections_.erase(conn->name());

  EventLoop *io_loop = conn->io_loop();
  io_loop->add_functor_queue(
          bind(&Connection::connection_destroyed, conn));
}

}  // namespace cyclone
