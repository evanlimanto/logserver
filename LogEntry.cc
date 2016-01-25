/*
 * LogEntry.cc
 * Copyright (C) 2016 evanlimanto <evanlimanto@gmail.com>
 *
 * Distributed under terms of the MIT license.
 */

#include "./LogEntry.h"

LogEntry::LogEntry() {
}

LogEntry::LogEntry(const LogEntry &other) :
  size_(other.size_), offset_(other.offset_), timestamp_(other.timestamp_),
  data_(other.data_), hasData_(other.hasData_) {
}

LogEntry::LogEntry(int _size, int _offset, int64 _timestamp,
                   const string &_data) :
  size_(_size), offset_(_offset), timestamp_(_timestamp),
  data_(_data), hasData_(true) {
}

LogEntry::~LogEntry() noexcept {
}
