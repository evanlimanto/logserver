/*
 * LogManager.cc
 * Copyright (C) 2016 evanlimanto <evanlimanto@gmail.com>
 *
 * Distributed under terms of the MIT license.
 */

#include "./LogManager.h"

#include <boost/filesystem.hpp>
#include <fstream>
#include <memory>

#include "./utils.h"
#include "./LogIndex.h"
#include "./LogFile.h"
#include "./LogEntry.h"

int main(int argc, char **argv) {
  vector<pair<string, string>> args;
  utils::argparse(&args, argc, argv);

  utils::kGen.seed(time(0));
  LogIndex index(true);
  for (const auto &argpair : args) {
    if (argpair.first == "write") {
      index.addEntryToIndex(argpair.second);
    } else if (argpair.first == "dump") {
      index.dumpEntriesToStdout();
    }
  }

  return 0;
}
