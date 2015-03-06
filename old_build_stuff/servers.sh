#!/bin/sh
make -f Makefile.ky CXXFLAGS=-DSIM $1 && make -f Makefile.ky servers
