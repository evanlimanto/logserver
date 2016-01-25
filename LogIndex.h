/*
 * LogIndex.h
 * Copyright (C) 2016 evanlimanto <evanlimanto@gmail.com>
 *
 * Distributed under terms of the MIT license.
 */

#ifndef LOGINDEX_H_
#define LOGINDEX_H_

#include "LogEntry.h"
#include "LogFile.h"

#include <string>
#include <vector>

using std::string;
using std::vector;

const char kIndexFilePrefix[] = "logServerIndex";

/* Index File Format
 * LogfileName LogfileSize LogfileNumEntries
 * offset1 size1 timestamp1
 * offset2 size2 timestamp2
 * ...
 * offsetN sizeN timestampN
 */
class LogIndex {
 public:
  explicit LogIndex(bool readEntryData = false);
  LogIndex(const LogIndex &other);
  ~LogIndex() noexcept;
  LogIndex& operator =(const LogIndex &other);
  void flush();
  void read(bool readEntryData = false);
  void addEntryToIndex(const string &data);
  void addLogFileToIndex(LogFile *logfile);
  void dumpEntriesToStdout();
  vector<LogFile*> &logs() {
    return logs_;
  }

 private:
  vector<LogFile*> logs_;
};

#endif  // LOGINDEX_H_
