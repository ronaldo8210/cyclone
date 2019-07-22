/***********************************************************************
#   > File Name   : include/callbacks.hpp
#   > Author      : ronaldo
#   > Description : 
#   > Create Time : 2019-05-09 16:04:47
***********************************************************************/
#pragma once

#include <functional>
#include <memory>

namespace cyclone {

class Connection;
typedef std::shared_ptr<Connection> connection_ptr;
typedef std::function<void (const connection_ptr&)> connect_callback;
typedef std::function<void (const connection_ptr&)> message_callback;
typedef std::function<void (const connection_ptr&)> write_complete_callback;
typedef std::function<void (const connection_ptr&)> close_callback;

}  // namespace cyclone
