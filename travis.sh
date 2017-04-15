#!/usr/bin/env sh
# Script for Building and testing on travis-ci


mkdir build
cd build
cmake ..
make
ctest -N
