#!/bin/bash
mkdir -p build/pi
cd build/pi
cmake -DCMAKE_TOOLCHAIN_FILE=cmake/toolchains/pi.osx.cmake ../..
make VERBOSE=1 -j3
