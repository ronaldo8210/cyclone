/***********************************************************************
#   > File Name   : buffer.hpp
#   > Author      : ronaldo
#   > Description : 
#   > Create Time : 2019-07-18 21:32:51
***********************************************************************/
#pragma once

#include <assert.h>
#include <algorithm>
#include <string>
#include <vector>

namespace cyclone {

using namespace std;

class Buffer 
{
 public:
  explicit Buffer(size_t initial_size = 1024)
    : buffer_(initial_size), read_index_(0), write_index_(0) {
    assert(readable_bytes() == 0);
    assert(writable_bytes() == initial_size);
  }

  size_t readable_bytes() const { return write_index_ - read_index_; }

  size_t writable_bytes() const { return buffer_.size() - write_index_; }

  // size_t prependable_bytes() const { return read_index_; }

  const char* peek() const { return begin() + read_index_; }

  void retrieve(size_t len) {
    assert(len <= readable_bytes());
    if (len < readable_bytes()) {
      read_index_ += len;
    } else {
      retrieve_all();
    }
  }

  void retrieve_all() {
    read_index_ = 0;
    write_index_ = 0;
  }

  string retrieve_all_as_string() {
    return retrieve_as_string(readable_bytes());
  }

  string retrieve_as_string(size_t len) {
    assert(len <= readable_bytes());
    string result(peek(), len);
    retrieve(len);
    return result;
  }

  void append(const char* data, size_t len) {
    ensure_writable_bytes(len);
    std::copy(data, data + len, begin_write());
    has_written(len);
  }

  void ensure_writable_bytes(size_t len) {
    if (writable_bytes() < len) {
      make_space(len);
    }
    assert(writable_bytes() >= len);
  }

  char* begin_write() { return begin() + write_index_; }

  const char* begin_write() const { return begin() + write_index_; }

  void has_written(size_t len) {
    assert(len <= writable_bytes());
    write_index_ += len;
  }

  void unwrite(size_t len) {
    assert(len <= readable_bytes());
    write_index_ -= len;
  }

  void shrink(size_t reserve) {

  }

  size_t capacity() const { return buffer_.capacity(); }

  ssize_t read_fd(int fd, int *saved_errno);

 private:
  char* begin() { return &*buffer_.begin(); }

  const char* begin() const { return &*buffer_.begin(); }

  void make_space(size_t len);

  std::vector<char> buffer_;

  size_t read_index_;

  size_t write_index_;
};

}  // namespace cyclone
