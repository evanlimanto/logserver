/*
 * Copyright 2016 Evan Limanto
 */

#include "./log_manager.h"

#include <boost/filesystem.hpp>
#include <fstream>
#include <memory>

#include "./utils.h"

using boost::archive::text_oarchive;
using boost::archive::text_iarchive;
using boost::filesystem::exists;
using std::istream;
using std::ofstream;
using std::unique_ptr;

LogIndex::LogIndex(bool readEntryData) {
  if (exists(kIndexFilePrefix)) {
    read(readEntryData);
  }
}

LogIndex::~LogIndex() {
  flush();
  for (LogFile *log : logs_) {
    delete log;
  }
  logs_.clear();
}

void LogIndex::read(bool readEntryData) {
  fstream stream(kIndexFilePrefix, fstream::in);
  LogFile *log = new LogFile();
  while (stream >> log->filename()) {
    if (utils::is_log_file(log->filename())) {
      stream >> log->size() >> log->num_entries();
      for (int i = 0; i < log->num_entries(); ++i) {
        LogEntry *entry = new LogEntry();
        stream >> entry->offset();
        stream >> entry->size();
        stream >> entry->timestamp();
        if (readEntryData) {
          utils::readDataFromFile(&entry->data(), log->filename(),
                                  entry->offset(), entry->size());
          entry->hasData_ = true;
        }
        log->addEntryToFile(entry, false);
      }
      stream.close();
    } else {
      fprintf(stderr, "File %s is not a valid log.\n", log->filename().c_str());
    }
  }
  addLogFileToIndex(log);
}

void LogIndex::flush() {
  fstream stream(kIndexFilePrefix, fstream::out);
  for (LogFile *log : logs_) {
    stream << log->filename();
    stream << " ";
    stream << log->size();
    stream << " ";
    stream << log->num_entries();
    stream << "\n";
    for (int i = 0; i < log->num_entries(); i++) {
      LogEntry *entry = log->entries_[i];
      stream << entry->offset();
      stream << " ";
      stream << entry->size();
      stream << " ";
      stream << entry->timestamp();
      stream << "\n";
    }
  }
  stream.close();
}

void LogIndex::dumpEntriesToStdout() {
  for (LogFile *log : logs_) {
    for (LogEntry *entry : log->entries_) {
      printf("log entry: %s\n", entry->data().c_str());
    }
  }
}

void LogIndex::addEntryToIndex(const string &data) {
  if (logs_.empty() || logs_.back()->size() + static_cast<int>(data.length()) >
                       utils::kMaxLogFileSize) {
    // Create new log file
    LogEntry *entry = new LogEntry(static_cast<int>(data.length()), 0,
                                   utils::get_current_timestamp(), data);
    LogFile *logfile = new LogFile();
    logfile->addEntryToFile(entry, true);
    addLogFileToIndex(logfile);
  } else {
    // Use existing log file
    LogEntry *entry = new LogEntry(
        static_cast<int>(data.length()), logs_.back()->size(),
        utils::get_current_timestamp(), data);
    logs_.back()->addEntryToFile(entry, true);
  }
}

void LogIndex::addLogFileToIndex(LogFile *logfile) {
  logs_.push_back(logfile);
}

void LogFile::addEntryToFile(LogEntry *entry, bool isNewEntry) {
  logfile_lock_.lock();
  if (filename().empty()) {
    utils::generateLogFile(this);
  }
  entries_.push_back(entry);
  if (isNewEntry) {
    size_ += entry->size();
    num_entries_ += 1;
  }
  logfile_lock_.unlock();
}

LogFile::~LogFile() {
  for (LogEntry *entry : entries_) {
    if (entry->hasData()) {
      utils::writeDataToFile(entry->data(), filename(),
                             entry->offset(), entry->size());
    }
    delete entry;
  }
  entries_.clear();
}

LogEntry::LogEntry(int _size, int _offset, int64 _timestamp,
                   const string &_data) :
  size_(_size), offset_(_offset), timestamp_(_timestamp),
  data_(_data), hasData_(true) {}

int main(int argc, char **argv) {
  utils::kGen.seed(time(0));
  LogIndex index(true);
  index.dumpEntriesToStdout();

  for (int i = 1; i < argc; i++) {
    printf("argv[%d]: %s\n", i, argv[i]);
    index.addEntryToIndex(argv[i]);
  }

  return 0;
}
