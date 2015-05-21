#!/bin/sh
DIRECTORY=`dirname $1`
TARGET=`basename $1|cut -d'.' -f1 | sed -e "s|/|_|g"`

HOST=fixme
#10.0.1.66 #hmmm, could init_session to set this (call init_session.sh 10.0.1.66)

echo DEPLOYING $TARGET to $HOST
ssh -p 22 $HOST  "rm -f /tmp/$TARGET"
scp -P 22 build/pi/$TARGET $HOST:/tmp
echo RUNNING $TARGET on $HOST
ssh -t -p 22 $HOST /tmp/$TARGET
