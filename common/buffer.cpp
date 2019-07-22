/***********************************************************************
#   > File Name   : buffer.cpp
#   > Author      : ronaldo
#   > Description : 
#   > Create Time : 2019-07-20 16:41:10
***********************************************************************/

#include <buffer.hpp>

#include <errno.h>
#include <sys/uio.h>
#include <sock_ops.hpp>

namespace cyclone {

void Buffer::make_space(size_t len) {
  if (writable_bytes() < len) {
    buffer_.resize(write_index_ + len);
  } else {
    size_t readable = readable_bytes();
    std::copy(begin()+read_index_, begin()+write_index_, begin());
    read_index_ = 0;
    write_index_ = read_index_ + readable;
    assert(readable == readable_bytes());
  }
}

// 从fd读取数据，放入缓存，供应用层程序读取
ssize_t Buffer::read_fd(int fd, int *saved_errno) {
  // readv在一个原子操作中将fd inode内核缓存中的数据读取到多个应用层缓冲区
  // 借用栈上的内存，一次从fd尽量多读一些数据，优化性能

  char extra_buf[1024] = {0};
  struct iovec vec[2];

  const size_t writable = writable_bytes();
  vec[0].iov_base = begin() + write_index_;
  vec[0].iov_len = writable;
  vec[1].iov_base = extra_buf;
  vec[1].iov_len = sizeof(extra_buf);

  // 判断是否需要使用栈上内存，如果buffer本身可写空间足够，不使用栈上内存
  const int iovcnt = (writable < 1024) ? 2 : 1;
  const ssize_t n = sockops::readv(fd, vec, iovcnt);  // 返回时数据已被写入buffer
  if (n < 0) {
    *saved_errno = errno;
  } else if (static_cast<size_t>(n) <= writable) {
    write_index_ += n;
  } else {
    write_index_ = buffer_.size();
    append(extra_buf, n - writable);
  }

  return n;
}

}  // namespace cyclone
