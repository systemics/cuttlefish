#!/bin/bash


DIRECTORY=`echo $1 |cut -d'/' -f1`
FILENAME=`echo $1 |cut -d'/' -f2 |cut -d'.' -f1 | sed -e "s|/|_|g"`
TARGET=${FILENAME}
echo Directory is $DIRECTORY
echo Filename is $FILENAME
echo Target Name is $TARGET

echo Deploying $TARGET to Host
make deploy $1 HOST=root@10.0.1.66 NAME=$TARGET

