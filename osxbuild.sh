#!/bin/bash

#build project on and control from your osx laptop
mkdir -p build/osx
cd build/osx
cmake ../..
#cmake -DCMAKE_TOOLCHAIN_FILE=cmake/toolchains/osx.cmake ..
make VERBOSE=1 -j3
