#!/bin/sh
make -f Makefile.ky CXXFLAGS=-DSIM $1 && copy build/bin/main 192.168.7.2
make -f Makefile.ky                $1 && copy build/bin/main 192.168.7.4 192.168.7.5 192.168.7.6 192.168.7.7 192.168.7.8 192.168.7.9 192.168.7.10 192.168.7.11 192.168.7.12 192.168.7.13 192.168.7.14 192.168.7.15 192.168.7.16 192.168.7.17 192.168.7.18
