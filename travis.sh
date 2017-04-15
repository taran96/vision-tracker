#!/usr/bin/env sh
# Script for Building and testing on travis-ci


mkdir build || true
cd build
cmake ..
make
make test
