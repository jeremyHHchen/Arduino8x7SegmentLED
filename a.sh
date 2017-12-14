#!/bin/bash

echo $(date "+%H:%M:%S" | awk -F: ' { print $1*3600+$2*60+$3 } ') |  picocom -b 9600 /dev/ttyACM0  -r -
