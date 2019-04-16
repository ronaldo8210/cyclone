/***********************************************************************
#   > File Name   : connection.hpp
#   > Author      : ronaldo
#   > Description : 
#   > Create Time : 2019-04-15 22:59:14
***********************************************************************/

namespace cyclone {

namespace io {

class EventLoop;

class Connection : boost::noncopyable {
 public:
  Connection();
  ~Connection();

 private:
};

}  // namespace io

}  // namespace cyclone
