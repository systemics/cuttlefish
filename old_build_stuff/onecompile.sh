#!/bin/bash


DIRECTORY=`echo $1 |cut -d'/' -f1`
FILENAME=`echo $1 |cut -d'/' -f2 |cut -d'.' -f1 | sed -e "s|/|_|g"`
TARGET=${FILENAME}
echo Directory is $DIRECTORY
echo Filename is $FILENAME
echo Target Name is $TARGET


echo Compiling $1 to $TARGET
make $1 NAME=$TARGET
echo Deploying $TARGET to Host

#make copy NAME=$TARGET
#make many NAME=$TARGET

make deploy $1 HOST=root@30.cf NAME=$TARGET
