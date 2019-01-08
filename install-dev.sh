#!/bin/sh

mkdir ~/wroot
cd ~/wroot

sudo apt-get update

sudo apt-get install -y build-essential
sudo apt-get install -y libc6-dev libncursesw5-dev
sudo apt-get install -y gdb gdbserver
sudo apt-get install -y qt5-default

#GCC 6.3.0>
sudo apt-get install build-essential software-properties-common -y && \
sudo add-apt-repository ppa:ubuntu-toolchain-r/test -y && \
sudo apt-get update && \
sudo apt-get install cmake -y && \
sudo apt-get install gcc-snapshot -y && \
sudo apt-get install gcc-6 g++-6 -y && \
sudo update-alternatives --install /usr/bin/gcc gcc /usr/bin/gcc-6 60 --slave /usr/bin/g++ g++ /usr/bin/g++-6 && \
sudo apt-get install gcc-4.8 g++-4.8 -y && \
sudo update-alternatives --install /usr/bin/gcc gcc /usr/bin/gcc-4.8 60 --slave /usr/bin/g++ g++ /usr/bin/g++-4.8;

#APACHE UTILS
sudo apt install apache2-utils -y

#ZLIB
sudo wget http://zlib.net/zlib-1.2.11.tar.gz
tar -xzf zlib-1.2.11.tar.gz zlib-1.2.11/
cd zlib-1.2.11/
./configure
make
sudo make install