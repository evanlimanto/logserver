/*
 * LogEntry.h
 * Copyright (C) 2016 evanlimanto <evanlimanto@gmail.com>
 *
 * Distributed under terms of the MIT license.
 */

#ifndef LOGENTRY_H_
#define LOGENTRY_H_

#include <string>

#include "./utils.h"

using std::string;

class LogEntry {
 public:
  LogEntry();
  LogEntry(const LogEntry &other);
  LogEntry(int _size, int _offset, int64 _timestamp, const string &_data);
  ~LogEntry() noexcept;
  LogEntry& operator =(const LogEntry &other);
  string& data() {
    return data_;
  }
  int& offset() {
    return offset_;
  }
  int& size() {
    return size_;
  }
  int64& timestamp() {
    return timestamp_;
  }
  bool& hasData() {
    return hasData_;
  }

 private:
  int size_;
  int offset_;
  int64 timestamp_;
  string data_;
  bool hasData_;

  friend class LogFile;
  friend class LogIndex;
};

#endif  // LOGENTRY_H_
