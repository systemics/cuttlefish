#!/bin/bash

#build project on and control from your osx laptop
mkdir -p build/jetson
cd build/jetson
cmake ../.. -DJETSON=1
make VERBOSE=1 -j7
