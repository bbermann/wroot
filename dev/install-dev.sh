#!/bin/sh

mkdir ~/wroot
cd ~/wroot

apt-get update
apt-get install -y man build-essential software-properties-common wget

# GCC 6.3.0 (wroot build) + 4.8 (for building old libs)
add-apt-repository ppa:ubuntu-toolchain-r/test -y && \
apt-get update && \
apt-get install cmake -y && \
apt-get install gcc-6 g++-6 -y && \
update-alternatives --install /usr/bin/gcc gcc /usr/bin/gcc-6 60 --slave /usr/bin/g++ g++ /usr/bin/g++-6 && \
apt-get install gcc-4.8 g++-4.8 -y && \
update-alternatives --install /usr/bin/gcc gcc /usr/bin/gcc-4.8 60 --slave /usr/bin/g++ g++ /usr/bin/g++-4.8;
apt-get install -y libc6-dev gdb gdbserver

# APACHE UTILS
# apt install apache2-utils -y

# ZLIB
wget http://zlib.net/zlib-1.2.11.tar.gz
tar -xzf zlib-1.2.11.tar.gz zlib-1.2.11/
cd zlib-1.2.11/
./configure
make
make install