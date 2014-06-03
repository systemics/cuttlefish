#!/bin/sh
DIRECTORY=`echo $1 |cut -d'/' -f1`
FILENAME=`echo $1 |cut -d'/' -f2 |cut -d'.' -f1 | sed -e "s|/|_|g"`
TARGET=${FILENAME}
echo Directory is $DIRECTORY
echo Filename is $FILENAME
echo Target Name is $TARGET

echo Compiling $1 as $TARGET to RENDERERS 
make $1 NAME=$TARGET && make copy NAME=$TARGET #&& make many NAME=$TARGET

echo Compiling $1 as $TARGET to SIMULATOR
#make $1 NAME=$TARGET SIMBUILD=1 
#make deploy $1 HOST=pi@192.168.7.2 NAME=$TARGET




