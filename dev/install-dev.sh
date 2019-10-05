#!/bin/bash

wroot_dir="$(dirname $0)"
cd $wroot_dir/../
wroot_dir=$PWD

# WARNING: Do not change this file without also changing Dockerfile.

# Sets the install wroot working directory temporarily
sudo mkdir /tmp/wroot/install
cd /tmp/wroot/install

sudo apt-get update && apt-get install -y \
man \
apt-transport-https \
ca-certificates \
gnupg \
wget \
build-essential \
software-properties-common \
libreadline-dev \
apache2-utils \
nano \
htop \
psmisc

wget -O - https://apt.kitware.com/keys/kitware-archive-latest.asc 2>/dev/null | sudo apt-key add - && \
sudo apt-add-repository 'deb https://apt.kitware.com/ubuntu/ bionic main' && \
sudo apt-get update && \
sudo apt-get install cmake -y

# GCC 6.3.0 (wroot build) + 4.8 (for building old libs)
sudo add-apt-repository ppa:ubuntu-toolchain-r/test -y && apt-get update && apt-get install -y \
gcc-9 g++-9 \
libc6-dev \
gdb \
gdbserver \
gcc-4.8 \
g++-4.8

sudo update-alternatives --install /usr/bin/gcc gcc /usr/bin/gcc-9 90 --slave /usr/bin/g++ g++ /usr/bin/g++-9

# ZLIB
sudo wget http://zlib.net/zlib-1.2.11.tar.gz && \
tar -xzf zlib-1.2.11.tar.gz zlib-1.2.11/ && \
cd zlib-1.2.11/ && \
./configure && \
make && \
make install

# BOOST
sudo apt-get update && sudo apt-get install -y libboost-all-dev

# Change directory to the actual wroot working directory
cd $wroot_dir