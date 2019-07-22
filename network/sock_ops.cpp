/***********************************************************************
#   > File Name   : sock_ops.cpp
#   > Author      : ronaldo
#   > Description : 
#   > Create Time : 2019-07-10 16:57:00
***********************************************************************/

#include <sock_ops.hpp>

#include <types.hpp>
#include <errno.h>
#include <fcntl.h>
#include <sys/socket.h>
#include <sys/uio.h>
#include <unistd.h>

namespace cyclone {

namespace sockops {

int create_nonblocking(sa_family_t family) {
  int sockfd = ::socket(family, SOCK_STREAM, IPPROTO_TCP);
  if (sockfd < 0) {
  
  }

  set_nonblocking(sockfd);

  return sockfd;
}

void set_nonblocking(int sockfd) {
  int flags = ::fcntl(sockfd, F_GETFL, 0);
  flags |= O_NONBLOCK;
  int ret = ::fcntl(sockfd, F_SETFL, flags);
  (void)ret;
}

int connect(int sockfd, const struct sockaddr *addr) {
  return ::connect(sockfd, addr, static_cast<socklen_t>(sizeof(struct sockaddr_in6)));
}

void bind(int sockfd, const struct sockaddr *addr) {
  int ret = ::bind(sockfd, addr, static_cast<socklen_t>(sizeof(struct sockaddr_in6)));
  if (ret < 0) {

  }
}

void listen(int sockfd) {
  int ret = ::listen(sockfd, SOMAXCONN);
  if (ret < 0) {

  }
}

int accept(int sockfd, struct sockaddr_in6 *addr) {
  socklen_t addrlen = static_cast<socklen_t>(sizeof *addr);
  int connfd = ::accept(sockfd, sockaddr_cast(addr), &addrlen);
  if (connfd > 0) {
    set_nonblocking(connfd);
    return connfd;
  } else {
    return -1;  
  }
}

ssize_t read(int sockfd, void *buf, size_t count) {
  return ::read(sockfd, buf, count);
}

ssize_t readv(int sockfd, const struct iovec *iov, int iovcnt) {
  return ::readv(sockfd, iov, iovcnt);
}

ssize_t write(int sockfd, const void *buf, size_t count) {
  return ::write(sockfd, buf, count);
}

void close(int sockfd) {
  if (::close(sockfd) < 0) {

  }
}

void shutdown(int sockfd) {
  if (::shutdown(sockfd, SHUT_WR) < 0) {
      
  }
}

struct sockaddr* sockaddr_cast(struct sockaddr_in6 *addr) {
  return static_cast<struct sockaddr*>(implicit_cast<void*>(addr));
}

const struct sockaddr* sockaddr_cast(const struct sockaddr_in6 *addr) {
  return static_cast<const struct sockaddr*>(implicit_cast<const void*>(addr));
}

}  // namespace sockops

}  // namespace cyclone
