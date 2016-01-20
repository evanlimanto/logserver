/*
 * Copyright 2016 Evan Limanto
 */

#ifndef LOG_MANAGER_H_
#define LOG_MANAGER_H_

#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <boost/serialization/access.hpp>
#include <boost/serialization/string.hpp>
#include <mutex>
#include <string>
#include <vector>

using std::mutex;
using std::string;
using std::vector;

typedef long long int64;

const char kIndexFilePrefix[] = "logServerIndex";

class LogIndex;
class LogFile;
class LogEntry;

class LogIndex {
 public:
  LogIndex();
  ~LogIndex();
  void flush();
  void read();
  void addEntryToIndex(const string &data);
  void addLogFileToIndex(const LogFile &logfile);

 private: 
  vector<LogFile> logs_;
  template <class Archive>
  void serialize(Archive &ar, const unsigned int version) {
    for (LogFile &log : logs_) {
      ar & log;
    }
  }

  friend class boost::serialization::access;
};

class LogEntry {
 public:
  LogEntry(int _size, int _offset, int64 _timestamp, const string &_data);
  ~LogEntry() {}

 private:
  int size_;
  int offset_;
  int64 timestamp_;
  string data_;

  friend class LogFile;
};

class LogFile {
 public:
  LogFile() {}
  LogFile(const LogFile &other) : filename_(other.filename_), size_(other.size_),
    entries_(other.entries_) {}
  ~LogFile() {}
  void addEntryToFile(const LogEntry &entry);
  int size() {
    return size_;
  }

 private:
  string filename_;
  int size_;  // log file size in bytes.
  vector<LogEntry> entries_;
  mutex logfile_lock_;

  template <class Archive>
  void serialize(Archive &ar, const unsigned int version) {
    ar & filename_;
    ar & size_;
    for (LogEntry &entry : entries_) {
      ar & entry.offset_;
      ar & entry.size_;
      ar & entry.timestamp_;
    }
  }

  friend class boost::serialization::access;
  friend class LogIndex;
};


#endif  // LOG_MANAGER_H_
