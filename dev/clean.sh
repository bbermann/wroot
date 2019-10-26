#!/bin/bash

wroot_dir="$(dirname $0)"
cd $wroot_dir/../
wroot_dir=$PWD

cd $wroot_dir/bin

rm *.o > /dev/null 2>&1
rm Makefile > /dev/null 2>&1
rm cmake_install.cmake > /dev/null 2>&1
rm CMakeCache.txt > /dev/null 2>&1
rm -rf CMakeFiles > /dev/null 2>&1