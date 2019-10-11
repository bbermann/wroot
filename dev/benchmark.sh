#!/bin/bash

./run.sh prod &
sleep 3 # seconds
# Fast test: Execute (-c) concurrent http request until (implicit by -t) 50k requests or (-t) seconds timeout.
ab -t 20 -c 50 http://127.0.0.1:8000/ > logs/ab_output.log 2>&1 &
# Long duration test: Execute (-c) concurrent http request until reach the total of (-n) requests.
# ab -n 1000000 -c 500 http://127.0.0.1:8000/ > logs/ab_output.log 2>&1 &
htop
killall ab
killall wroot
cat logs/ab_output.log