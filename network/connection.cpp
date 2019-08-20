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
#include <sock_ops.hpp>

namespace cyclone {

Connection::Connection(EventLoop *io_loop, int fd, string name,
        const InetAddress &local_addr, const InetAddress &peer_addr) 
    : io_loop_(io_loop), 
    socket_(new Socket(fd)), 
    channel_(new Channel(io_loop, fd)),
    name_(name),
    state_(kConnecting),
    local_addr_(local_addr), 
    peer_addr_(peer_addr) {
  channel_->set_read_cb(std::bind(&Connection::handle_read, this, placeholders::_1));
  channel_->set_write_cb(std::bind(&Connection::handle_write, this));
  channel_->set_close_cb(std::bind(&Connection::handle_close, this));
  channel_->set_error_cb(std::bind(&Connection::handle_error, this));
}

Connection::~Connection() {
  if (state_ != kDisconnected) {
    // 严重错误日志
  }
}

// 在worker线程中调用
void Connection::send(const char *message, size_t len) {
  assert(state_ == kConnected);

  if (io_loop_->is_in_loop_thread()) {
    // 没有使用worker线程池，fd上的数据读写和业务计算都在io线程完成
    send_in_io_loop(message, len);
  } else {
    // 业务计算在worker线程完成，为避免读写send_buffer的竞态，将写入操作放到io_loop的任务队列中
    io_loop_->add_functor_queue(
        bind(&Connection::send_in_io_loop, this, message, len));
  } 
}

void Connection::send(Buffer *buffer) {

}

// 在worker线程中调用
// 调用shutdown关闭服务器写端的场景：
// 1、短连接，一次连接就处理一个任务，调用send完后就可以调用shutdown
// 2、长连接，如果出现非法数据，服务器可以选择用shutdown关闭写功能
void Connection::shutdown() {
  if (state_ = kConnected) {
    set_state(kDisConnecting);
    io_loop_->add_functor_queue(bind(&Connection::shutdown_in_io_loop, this));
  }
}

void Connection::force_close() {

}

void Connection::connection_established() {
  io_loop_->assert_in_loop_thread();
  assert(state_ == kConnecting);

  set_state(kConnected);
  channel_->tie(shared_from_this());
  channel_->enable_reading();

  connect_cb_(shared_from_this());
}

void Connection::connection_destroyed() {
  io_loop_->assert_in_loop_thread();
  assert(state_ == kConnected);

  set_state(kDisconnected);
  channel_->disable_all();
  channel_->remove();
}

void Connection::handle_read(Timestamp recv_time) {
  io_loop_->assert_in_loop_thread();

  int saved_errno = 0;
  ssize_t n = recv_buffer_.read_fd(channel_->fd(), &saved_errno);
  if (n > 0) {
    // 调用用户设置的应用层回调函数，判断是否读到完整的应用层数据包
    // 一次实际从fd读多少数据由内核控制，应用层回调函数去做拆包判断
    message_cb_(shared_from_this(), &recv_buffer_, recv_time);
  } else if (n == 0) {
    // 对端关闭了tcp连接，fd不再可读
    handle_close();
  } else {
    errno = saved_errno;
    handle_error();
  }
}

void Connection::handle_write() {
  io_loop_->assert_in_loop_thread();

  if (channel_->is_writing()) {
    ssize_t n = sockops::write(channel_->fd(), send_buffer_.peek(), send_buffer_.readable_bytes());
    if (n > 0) {
      send_buffer_.retrieve(n);
      if (0 == send_buffer_.readable_bytes()) {
        // 应用层发送缓存中的数据都写入fd的内核发送缓存了
        // 暂时不再关注fd上的写事件，避免busy loop
        channel_->disable_writing();
        if (write_complete_cb_) {
          //write_complete_cb_(shared_from_this());
          io_loop_->add_functor_queue(bind(write_complete_cb_, shared_from_this()));
        }

        if (state_ == kDisConnecting) {
          // 之前有过半关闭的动作，但由于还有数据未写完，所以当时没有调用shutdown，只设置了状态为DisConnecting
          // 现在数据写完了，可以调用shutdown了
          // 要保证所有对fd的操作都在io线程完成
          shutdown_in_io_loop();
        }
      }
    } else {
      // 错误日志    
    }
  } else {
    // 错误日志    
  }
}

void Connection::handle_close() {
  io_loop_->assert_in_loop_thread();
  assert(state_ == kDisconnected || state_ == kDisConnecting);

  set_state(kDisConnecting);
  channel_->disable_all();

  close_cb_(shared_from_this());
}

void Connection::handle_error() {

}

void Connection::send_in_io_loop(const char *data, size_t len) {
  assert(io_loop_->is_in_loop_thread());

  if (state_ != kConnected) {
    return;
  }

  ssize_t nwrote = 0;
  size_t remain;

  if (!channel_->is_writing() && send_buffer_.readable_bytes() == 0) {
    // 如果输出缓冲区的数据都已经写完，且fd没有关注写事件，则尝试直接将数据写入fd  
    nwrote = sockops::write(channel_->fd(), data, len);
    if (nwrote < 0) {

      // 错误日志并退出

    } else {
      remain = len - nwrote;
      if (remain == 0 && write_complete_cb_) {
        // 数据已全部写完，可以直接调用写完回调，也可以将回调操作放到io_loop里去执行
        // 一定要用shared_from_this，防止回调函数被调用时，本Connection实例被析构
        io_loop_->add_functor_queue(bind(write_complete_cb_, shared_from_this()));
        return;
      }
    } 
  }

  // 将剩余的未直接写入fd的数据追加到输出缓存
  send_buffer_.append(data + nwrote, remain);
  if (!channel_->is_writing()) {
    channel_->enable_writing();
  }

  // 输出缓存占用空间达到警戒水位预警
  size_t old_len = send_buffer_.readable_bytes();
  if (old_len + remain >= highwater_mark_ && highwater_mark_cb_) {
    io_loop_->add_functor_queue(bind(highwater_mark_cb_, shared_from_this(), old_len + remain));    
  }
}

void Connection::shutdown_in_io_loop() {
  io_loop_->assert_in_loop_thread();
  // 如果发送缓存还有数据没有写入fd，则只讲状态置为DisConnecting，不能立即调用::shutdown
  // 在handle_write中将数据全部写入fd后会调用::shutdown
  if (!channel_->is_writing()) {
    socket_->shutdown();
  }
}

}  // namespace cyclone
