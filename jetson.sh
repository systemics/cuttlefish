#!/bin/sh

make -f Makefile.jetson CXXFLAGS=-DSIM $1
