#!/bin/bash

./run.sh prod &
sleep 5 # seconds
ab -n 100000 -c 50 http://127.0.0.1:8000/ > logs/ab_output.log 2>&1 &
sleep 1 # seconds
htop
killall ab
killall wroot