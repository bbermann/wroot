#!/bin/bash

./build.sh $1 &&
cd ../bin &&
gdbserver 10.0.75.1:666 ./wroot;