/*
 * Copyright 2016 Evan Limanto
 */

#include "./log_manager.h"
#include "./utils.h"

#include <boost/filesystem.hpp>
#include <fstream>

using boost::archive::text_oarchive;
using boost::archive::text_iarchive;
using boost::filesystem::exists;
using std::istream;
using std::ofstream;

LogIndex::LogIndex() {
  if (exists(kIndexFilePrefix)) {
    read();
  }
}

LogIndex::~LogIndex() {
  flush();
}

void LogIndex::read() {
  ifstream ifs(kIndexFilePrefix);
  text_iarchive ia(ifs); 
  ia >> *this;
}

void LogIndex::flush() {
  ofstream ofs(kIndexFilePrefix);
  text_oarchive oa(ofs);
  oa << *this;
}

void LogIndex::addEntryToIndex(const string &data) {
  if (logs_.empty() || logs_.back().size() + static_cast<int>(data.length()) > utils::kMaxLogFileSize) {
    // Create new log file
    LogEntry entry(static_cast<int>(data.length()), 0, utils::get_current_timestamp(), data); 
    LogFile logfile;
    logfile.addEntryToFile(entry);
    addLogFileToIndex(logfile);
  } else {
    // Use existing log file
    LogEntry entry(static_cast<int>(data.length()), logs_.size(), utils::get_current_timestamp(), data); 
    logs_.back().addEntryToFile(entry);
  }
}

void LogIndex::addLogFileToIndex(const LogFile &logfile) {
  logs_.push_back(logfile);
}

void LogFile::addEntryToFile(const LogEntry &entry) {
  entries_.push_back(entry);
}

LogEntry::LogEntry(int _size, int _offset, int64 _timestamp, const string &_data) : size_(_size), offset_(_offset), timestamp_(_timestamp), data_(_data) {
}

int main(int argc, char **argv) {
  vector<directory_entry> logs;
  utils::get_log_files(logs);

  LogIndex index;

  for (int i = 0; i < argc; i++) {
    printf("%s\n", argv[i]);
  }
  return 0;
}
