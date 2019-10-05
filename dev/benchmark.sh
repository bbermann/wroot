#!/bin/bash

./run.sh prod &
sleep 1 # seconds
ab -n 1000000 -c 50 -t 60 http://127.0.0.1:8000/404 > ab_output.log 2>&1 &
htop
killall wroot