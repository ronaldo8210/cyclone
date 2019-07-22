/***********************************************************************
#   > File Name   : connection.cpp
#   > Author      : ronaldo
#   > Description : 
#   > Create Time : 2019-04-15 22:59:36
***********************************************************************/

#include <connection.hpp>

#include <channel.hpp>
#include <event_loop.hpp>
#include <socket.hpp>

namespace cyclone {

Connection::Connection(EventLoop *io_loop, int fd, const InetAddress &local_addr, 
        const InetAddress &peer_addr) 
    : io_loop_(io_loop), 
    socket_(new Socket(fd)), 
    channel_(new Channel(io_loop, fd)), 
    state_(kConnecting),
    local_addr_(local_addr), 
    peer_addr_(peer_addr) {
  channel_->set_read_cb(std::bind(&Connection::handle_read, this));
  channel_->set_write_cb(std::bind(&Connection::handle_write, this));
  channel_->set_close_cb(std::bind(&Connection::handle_close, this));
  channel_->set_error_cb(std::bind(&Connection::handle_error, this));
}

Connection::~Connection() {
  if (state_ != kDisconnected) {
    // 严重错误日志
  }
}

void Connection::send(const char *message, size_t len) {
  /*
  if (io_loop_->is_in_loop_thread()) {
    // 没有使用worker线程池，fd上的数据读写和业务计算都在io线程完成
    send_in_io_loop(message, len)
  } else {
    // 业务计算在worker线程完成，为避免读写send_buffer的竞态，将写入操作放到io_loop的任务队列中
    io_loop_->add_functor_queue(
        std::bind(&Connection::send_in_io_loop), this, string(message));  // 拷贝一份参数（可改成移动，提高效率）
  } */
}

void Connection::send(Buffer *buffer) {

}

void Connection::shutdown() {

}

void Connection::force_close() {

}

void Connection::connection_established() {

}

void Connection::connection_destroyed() {

}

void Connection::handle_read() {

}

void Connection::handle_write() {

}

void Connection::handle_close() {

}

void Connection::handle_error() {

}

void Connection::send_in_io_loop(const char *data, size_t len) {
  assert(io_loop_->is_in_loop_thread());
  /*
  if (state_ != kConnected) {
    return;
  }

  ssize_t nwrote = 0;
  size_t remain;

  if (!channel_->is_writing() && send_buffer_.readablebytes() == 0) {
    // 如果输出缓冲区的数据都已经写完，且fd没有关注写事件，则尝试直接将数据写入fd  
    nwrote = sockops::write(channel_->sockfd(), data, len);
    if (nwrote == len) {
      if (write_complete_callback) {
        io_loop_->add_functor_queue();
      }
      return;
    }
  }

  remain = len - nwrote;
  send_buffer_.append(data+nwrote, remain);
  if (channel_->is_writing()) {

  } */
}

void Connection::shutdown_in_io_loop() {

}

}  // namespace cyclone
