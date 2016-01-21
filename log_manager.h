/*
 * Copyright 2016 Evan Limanto
 */

#ifndef LOG_MANAGER_H_
#define LOG_MANAGER_H_

#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <mutex>  // NOLINT
#include <string>
#include <vector>

using std::istream;
using std::mutex;
using std::ostream;
using std::string;
using std::vector;

typedef long long int64;  // NOLINT

const char kIndexFilePrefix[] = "logServerIndex";

class LogIndex;
class LogFile;
class LogEntry;

class LogEntry {
 public:
  LogEntry() {}
  LogEntry(int _size, int _offset, int64 _timestamp, const string &_data);
  ~LogEntry() {}
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
  int size_ = 0;
  int offset_ = 0;
  int64 timestamp_ = 0;
  string data_;
  bool hasData_;

  friend class LogFile;
  friend class LogIndex;
};

class LogFile {
 public:
  LogFile() {}
  LogFile(const LogFile &other) : filename_(other.filename_),
    size_(other.size_), entries_(other.entries_) {}
  ~LogFile();
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

 private:
  string filename_;
  int size_ = 0;  // log file size in bytes.
  int num_entries_ = 0;
  vector<LogEntry*> entries_;
  mutex logfile_lock_;
  friend class LogIndex;
};

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
  ~LogIndex();
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

#endif  // LOG_MANAGER_H_
