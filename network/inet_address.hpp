/***********************************************************************
#   > File Name   : InetAddress.hpp
#   > Author      : ronaldo
#   > Description : 
#   > Create Time : 2019-07-05 19:35:17
***********************************************************************/
#pragma once

namespace cyclone {

class InetAddress {
 public:
  InetAddress();

 private:
  union {
    struct sockaddr_in addr_;
    struct sockaddr_in6 addr6_;
  };
};

}  // namespace cyclone
