/*
 * Copyright 2016 Evan Limanto
 */

#ifndef UTILS_H_
#define UTILS_H_

#include <boost/algorithm/string/predicate.hpp>
#include <boost/date_time/posix_time/posix_time_types.hpp>
#include <boost/filesystem.hpp>
#include <fstream>
#include <vector>

using boost::algorithm::ends_with;
using boost::filesystem::current_path;
using boost::filesystem::directory_entry;
using boost::filesystem::directory_iterator;
using boost::filesystem::exists;
using boost::filesystem::is_directory;
using boost::filesystem::path;
using std::vector;
using std::ifstream;

namespace utils {

const string kLogExtension = ".log";
const int kMagicIdentifier = 0x7fffffff;
const int kMaxLogFileSize = 67108864;

static bool is_log_file(const directory_entry &entry) {
  // For now, log files are identified by the magic constant which is
  // appended to the end of the file.
  ifstream ifs(entry.path().native(), std::ifstream::binary);
  if (ifs) {
    ifs.seekg(-4, ifs.end);
    vector<char> result(4);
    ifs.read(&result[0], 4);  // big-endian
    return (((result[0] << 24) | (result[1] << 16) | (result[2] << 8) | result[3]) == kMagicIdentifier);
  }
  return false;
}

static void get_log_files(vector<directory_entry> &logs,
                          const path directory_path = current_path()) {
  if (exists(directory_path) && is_directory(directory_path)) {
    directory_iterator end_iter;
    for (directory_iterator iter(directory_path); iter != end_iter; ++iter) {
      directory_entry &entry = *iter;
      if (ends_with(entry.path().native(), kLogExtension) && is_log_file(entry)) {
        logs.emplace_back(entry);
      }
    }
  }
}

static int64 get_current_timestamp() {
  boost::posix_time::ptime time = boost::posix_time::microsec_clock::local_time();
  boost::posix_time::time_duration duration(time.time_of_day());
  return duration.total_milliseconds();
}

}  // namespace utils

#endif  // UTILS_H_
