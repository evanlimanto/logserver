/*
 * LogFile.cc
 * Copyright (C) 2016 evanlimanto <evanlimanto@gmail.com>
 *
 * Distributed under terms of the MIT license.
 */

#include "./LogFile.h"

#include <utility>

using std::move;

LogFile::LogFile() {
}

LogFile::LogFile(const LogFile &other) :
  filename_(other.filename_), size_(other.size_),
  num_entries_(other.num_entries_), entries_(other.entries_) {}

LogFile::~LogFile() noexcept {
  for (LogEntry *entry : entries_) {
    if (entry->hasData()) {
      utils::writeDataToFile(entry->data(), filename(),
                             entry->offset(), entry->size());
    }
    delete entry;
  }
  entries_.clear();
}

LogFile& LogFile::operator =(const LogFile &other) {
  LogFile temp(other);
  *this = move(temp);
  return *this;
}

void LogFile::addEntryToFile(LogEntry *entry, bool isNewEntry) {
  logfile_lock_.lock();
  if (filename().empty()) {
    LogFile::generateLogFile(this);
  }
  entries_.push_back(entry);
  if (isNewEntry) {
    size_ += entry->size();
    num_entries_ += 1;
  }
  logfile_lock_.unlock();
}

void LogFile::generateLogFile(LogFile *log) {
  do {
    utils::generateRandomAlphaString(&log->filename(),
                                     utils::kLogFileNameLength);
  } while (exists(log->filename()));
  fstream fs(log->filename(), fstream::out | fstream::binary);
  fs.write(utils::kMagicIdentifierStr, 4);
  fs.close();
}
