/*
 * LogFile.h
 * Copyright (C) 2016 evanlimanto <evanlimanto@gmail.com>
 *
 * Distributed under terms of the MIT license.
 */

#ifndef LOGFILE_H_
#define LOGFILE_H_

#include <memory>
#include <string>
#include <vector>

#include "./LogEntry.h"

using std::mutex;
using std::string;
using std::vector;

class LogFile {
 public:
  LogFile();
  LogFile(const LogFile &other);
  ~LogFile() noexcept;
  LogFile& operator =(const LogFile &other);
  void addEntryToFile(LogEntry *entry, bool isNewEntry);
  string& filename() {
    return filename_;
  }
  int& size() {
    return size_;
  }
  int& num_entries() {
    return num_entries_;
  }

  static void generateLogFile(LogFile *log);

 private:
  string filename_;
  int size_ = 0;  // log file size in bytes.
  int num_entries_ = 0;
  vector<LogEntry*> entries_;
  mutex logfile_lock_;
  friend class LogIndex;
};

#endif  // LOGFILE_H_
