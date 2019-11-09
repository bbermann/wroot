#!/bin/bash

wroot_dir="$(dirname $0)"
cd $wroot_dir/../
wroot_dir=$PWD

mkdir $wroot_dir/dev/cpplint > /dev/null 2>&1
cpplint $(find $wroot_dir/src -not \( -path $wroot_dir/src/3rdParty -prune \) -name *.cpp -or -name \*.hpp) > $wroot_dir/dev/cpplint/output.txt 2>&1