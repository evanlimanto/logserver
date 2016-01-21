/*
 * Copyright 2016 Evan Limanto
 */

#ifndef UTILS_H_
#define UTILS_H_

#include <boost/algorithm/string/predicate.hpp>
#include <boost/date_time/posix_time/posix_time_types.hpp>
#include <boost/filesystem.hpp>
#include <boost/random/mersenne_twister.hpp>
#include <boost/random/uniform_int_distribution.hpp>
#include <fstream>
#include <string>
#include <vector>

using boost::algorithm::ends_with;
using boost::filesystem::current_path;
using boost::filesystem::directory_entry;
using boost::filesystem::directory_iterator;
using boost::filesystem::exists;
using boost::filesystem::is_directory;
using boost::filesystem::path;
using std::vector;
using std::fstream;
using std::ifstream;
using std::ofstream;

namespace utils {

const char kLogExtension[] = ".log";
const int kMagicIdentifier = 0x7a5b1c4f;
const char kMagicIdentifierStr[] =
  {static_cast<char>(0x7a), static_cast<char>(0x5b),
  static_cast<char>(0x1c), static_cast<char>(0x4f)};
const int kMaxLogFileSize = 67108864;
const int kLogFileNameLength = 20;
boost::random::mt19937 kGen;

static bool is_log_file(const string &filename) {
  // For now, log files are identified by the magic constant which is
  // appended to the end of the file.
  fstream fs(filename, fstream::in | fstream::binary);
  if (fs) {
    char *buffer = new char[4];
    fs.read(buffer, 4);  // big-endian
    int fileIdentifier = 0;
    for (int i = 0; i < 4; i++) {
      fileIdentifier |= (static_cast<int>(buffer[i]) << ((3 - i) * 8));
    }
    delete[] buffer;
    fs.close();
    return (fileIdentifier == kMagicIdentifier);
  }
  return false;
}

static int64 get_current_timestamp() {
  boost::posix_time::ptime time =
    boost::posix_time::microsec_clock::local_time();
  boost::posix_time::time_duration duration(time.time_of_day());
  return duration.total_milliseconds();
}

static void readDataFromFile(string *data, const string &filename,
                             const int offset, const int size) {
  bool isLogFile = is_log_file(filename);
  fstream fs(filename, fstream::in | fstream::binary);
  if (fs) {
    if (isLogFile)
      fs.seekg(offset + sizeof(kMagicIdentifierStr), fstream::beg);
    else
      fs.seekg(offset, fstream::beg);
    char *buffer = new char[size + 1];
    fs.read(buffer, size);
    buffer[size] = '\0';
    *data = string(buffer);
    delete[] buffer;
    fs.close();
  }
}

static void writeDataToFile(const string &data, const string &filename,
                            const int offset, const int size) {
  bool isLogFile = is_log_file(filename);
  fstream fs(filename, fstream::in | fstream::out | fstream::binary);
  if (fs) {
    if (isLogFile)
      fs.seekp(offset + sizeof(kMagicIdentifierStr));
    else
      fs.seekp(offset);
    fs.write(data.c_str(), size);
    fs.seekp(0);
    fs.close();
  }
}

static char intToAlpha(int val) {
  if (val >= 0 && val <= 9) {
    return static_cast<char>(val + '0');
  } else if (val <= 35) {
    return static_cast<char>(val - 10 + 'A');
  } else {
    return static_cast<char>(val - 36 + 'a');
  }
}

static void generateRandomAlphaString(string *str, int length) {
  str->clear();
  boost::random::uniform_int_distribution<> dist(0, 61);
  for (int i = 0; i < length; i++) {
    str->push_back(intToAlpha(dist(kGen)));
  }
  str->append(kLogExtension);
}

static void generateLogFile(LogFile *log) {
  do {
    utils::generateRandomAlphaString(&log->filename(),
                                     utils::kLogFileNameLength);
  } while (exists(log->filename()));
  fstream fs(log->filename(), fstream::out | fstream::binary);
  fs.write(utils::kMagicIdentifierStr, 4);
  fs.close();
}

}  // namespace utils

#endif  // UTILS_H_
