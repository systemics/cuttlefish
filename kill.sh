#!/bin/bash

pssh -h tools/pi_host_names.txt "pkill -9 content"
