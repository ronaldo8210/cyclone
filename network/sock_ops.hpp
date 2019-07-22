/***********************************************************************
#   > File Name   : sock_ops.hpp
#   > Author      : ronaldo
#   > Description : 
#   > Create Time : 2019-07-10 16:56:50
***********************************************************************/
#pragma once

#include <arpa/inet.h>

namespace cyclone {

namespace sockops {

int create_nonblocking(sa_family_t family);

void set_nonblocking(int sockfd);

int connect(int sockfd, const struct sockaddr *addr);

void bind(int sockfd, const struct sockaddr *addr);

void listen(int sockfd);

int accept(int sockfd, struct sockaddr_in6 *addr);

ssize_t read(int sockfd, void *buf, size_t count);

ssize_t readv(int sockfd, const struct iovec *iov, int iovcnt);

ssize_t write(int sockfd, const void *buf, size_t count);

void close(int sockfd);

void shutdown(int sockfd);

struct sockaddr* sockaddr_cast(struct sockaddr_in6 *addr);

const struct sockaddr* sockaddr_cast(const struct sockaddr_in6 *addr);

}  // namespace sockops

}  // namespace cyclone
