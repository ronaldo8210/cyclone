/***********************************************************************
#   > File Name   : InetAddress.hpp
#   > Author      : ronaldo
#   > Description : 
#   > Create Time : 2019-07-05 19:35:17
***********************************************************************/
#pragma once

#include <netinet/in.h>

namespace cyclone {

// 前置声明
namespace sockops {
  const struct sockaddr* sockaddr_cast(const struct sockaddr_in6 *addr);
}

class InetAddress {
 public:
  InetAddress();

  const struct sockaddr* get_sock_addr() const {
    return sockops::sockaddr_cast(&addr6_);
  }

 private:
  union {
    struct sockaddr_in addr_;
    struct sockaddr_in6 addr6_;
  };
};

}  // namespace cyclone
