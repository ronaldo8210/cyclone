/***********************************************************************
#   > File Name   : include/event_loop.hpp
#   > Author      : ronaldo
#   > Description : 
#   > Create Time : 2019-04-13 21:38:27
***********************************************************************/

#include <boost/utility.hpp>

namespace cyclone{

class EventLoop : boost::noncopyable {
 public:
  EventLoop();
  ~EventLoop();

  void loop();
 private:
  bool looping_;
  // const pid_t thread_id_;
};

}  // namespace cyclone
