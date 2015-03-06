#!/bin/bash
mkdir -p build
cd build
cmake -DCMAKE_TOOLCHAIN_FILE=cmake/toolchains/pi.osx.cmake ..
make -j3
