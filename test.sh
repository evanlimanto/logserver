#! /bin/sh
#
# Copyright 2016 Evan Limanto
#

make clean || exit 1
make || exit 1
result=$(./logserver -write=a)
result=$(./logserver -write=b)
result=$(./logserver -write=c)
result=$(./logserver --dump)
if [ "$result" != "abc" ]; then
  echo $result
  exit 1
fi
