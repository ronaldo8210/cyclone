/***********************************************************************
#   > File Name   : types.hpp
#   > Author      : ronaldo
#   > Description : 
#   > Create Time : 2019-07-10 22:19:17
***********************************************************************/
#pragma once

namespace cyclone {

template<typename To, typename From>
inline To implicit_cast(From const &f) {
  return f;
}

}  // namespace cyclone
