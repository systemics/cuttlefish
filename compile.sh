#!/bin/bash


DIRECTORY=`echo $1 |cut -d'/' -f1`
FILENAME=`echo $1 |cut -d'/' -f2 |cut -d'.' -f1 | sed -e "s|/|_|g"`
TARGET=${FILENAME}
echo Directory is $DIRECTORY
echo Filename is $FILENAME
echo Target Name is $TARGET

echo Compiling $1 to $TARGET
make $1 NAME=$TARGET



