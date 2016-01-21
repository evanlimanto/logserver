#! /bin/sh
#
# test.sh
# Copyright (C) 2016 evanlimanto <evanlimanto@gmail.com>
#
# Distributed under terms of the MIT license.
#

make clean
rm *.log logServerIndex
make logserver
./logserver a
./logserver b
./logserver c
