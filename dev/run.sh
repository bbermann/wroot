#!/bin/bash

wroot_dir="$(dirname $0)"
cd $wroot_dir/../
wroot_dir=$PWD

$wroot_dir/dev/build.sh $1 && \
cd $wroot_dir/bin && \
echo "" && \
./wroot