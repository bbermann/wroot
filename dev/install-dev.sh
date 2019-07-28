#!/bin/sh

cd ../
wroot_dir=$PWD

# WARNING: Do not change this file without also changing Dockerfile.

# Sets the install wroot working directory temporarily
sudo mkdir /tmp/wroot/install
cd /tmp/wroot/install

sudo apt-get update && apt-get install -y \
man \
build-essential \
software-properties-common \
libreadline-dev \
wget \
apache2-utils \
nano

# GCC 6.3.0 (wroot build) + 4.8 (for building old libs)
sudo add-apt-repository ppa:ubuntu-toolchain-r/test -y && apt-get update && apt-get install -y \
cmake \
gcc-6 g++-6 \
libc6-dev \
gdb \
gdbserver \
gcc-4.8 \
g++-4.8

sudo update-alternatives --install /usr/bin/gcc gcc /usr/bin/gcc-6 60 --slave /usr/bin/g++ g++ /usr/bin/g++-6

# ZLIB
sudo wget http://zlib.net/zlib-1.2.11.tar.gz && \
tar -xzf zlib-1.2.11.tar.gz zlib-1.2.11/ && \
cd zlib-1.2.11/ && \
./configure && \
make && \
make install

# Change directory to the actual wroot working directory
cd $wroot_dir