#!/bin/bash

if [ -e "$1" ]
then
  pscp -h tools/pi_host_names.txt $1 /root/content
else
  echo $1 does not exist
fi
