/***********************************************************************
#   > File Name   : timestamp.cpp
#   > Author      : ronaldo
#   > Description : 
#   > Create Time : 2019-07-09 21:15:15
***********************************************************************/

#include <timestamp.hpp>

#include <sys/time.h>

namespace cyclone {

Timestamp now() {
  struct timeval tv;
  gettimeofday(&tv, NULL);
  int64_t seconds = tv.tv_sec;
  return Timestamp(seconds * 1000 * 1000 + tv.tv_usec);
}
    
}  // namespace cyclone
