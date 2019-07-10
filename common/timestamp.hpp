/***********************************************************************
#   > File Name   : timestamp.hpp
#   > Author      : ronaldo
#   > Description : 
#   > Create Time : 2019-07-09 15:15:42
***********************************************************************/
#pragma once

#include <algorithm>
#include <stdint.h>

namespace cyclone {

class Timestamp {
 public:
  Timestamp() : micro_seconds_since_epoch_(0) {

  }  

  // 禁止从int到Timestamp的隐式类型转换
  explicit Timestamp(uint64_t arg) : micro_seconds_since_epoch_(arg) {

  }

  void swap(Timestamp &other) {
    std::swap(micro_seconds_since_epoch_, other.micro_seconds_since_epoch_);
  }

  static Timestamp now();

 private:

  uint64_t micro_seconds_since_epoch_; 
};

}  // namespace cyclone
