#! /bin/sh
#
# Copyright 2016 Evan Limanto
#

make clean || exit 1
make logserver || exit 1
result=$(./logserver a)
result=$(./logserver b)
if [ "$result" != "a" ]; then
  echo $result
  exit 1
fi
result=$(./logserver)
if [ "$(./logserver c)" != "ab" ]; then
  exit 1
fi
result=$(./logserver)
if [ "$result" != "abc" ]; then
  echo $result
  exit 1
fi
