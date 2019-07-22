/***********************************************************************
#   > File Name   : nocopyable.hpp
#   > Author      : ronaldo
#   > Description : 
#   > Create Time : 2019-07-21 10:12:53
***********************************************************************/
#pragma once

namespace cyclone {

class nocopyable {
 public:
  nocopyable(nocopyable&) = delete;
  nocopyable& operator=(nocopyable&) = delete;
 protected:
  // 该类只允许作为其他类的父类
  // 不允许单独使用或作为其他类的成员
  nocopyable() {}
  ~nocopyable() {}
};

}  // namespace cyclone
