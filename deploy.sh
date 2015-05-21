#!/bin/sh
HOST=jetson

DIRECTORY=`dirname $1`
TARGET=`basename $1|cut -d'.' -f1 | sed -e "s|/|_|g"`

echo Deploying $TARGET in $DIRECTORY to jetson:/share folder

ssh -p 22 $HOST  "rm -f /share/$TARGET"
scp -P 22 build/pi/$TARGET $HOST:/share

#echo RUNNING $TARGET on $HOST
#ssh -t -p 22 $HOST /tmp/$TARGET
