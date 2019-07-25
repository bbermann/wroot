# Created and maintained by: Bruno dos Santos Bermann
# All rights reserved.

# Get the base Ubuntu image from Docker Hub
FROM ubuntu:18.04

# Install the working environment
# Generated from wroot/dev/install-dev.sh

# Sets the install wroot working directory temporarily
WORKDIR /tmp/wroot/install

RUN apt-get update && apt-get install -y \
man \
build-essential \
software-properties-common \
libreadline-dev \
wget \
apache2-utils \
nano

# GCC 6.3.0 (wroot build) + 4.8 (for building old libs)
RUN add-apt-repository ppa:ubuntu-toolchain-r/test -y && apt-get update && apt-get install -y \
cmake \
gcc-6 g++-6 \
libc6-dev \
gdb \
gdbserver \
gcc-4.8 \
g++-4.8

RUN update-alternatives --install /usr/bin/gcc gcc /usr/bin/gcc-6 60 --slave /usr/bin/g++ g++ /usr/bin/g++-6

# ZLIB
RUN wget http://zlib.net/zlib-1.2.11.tar.gz && \
tar -xzf zlib-1.2.11.tar.gz zlib-1.2.11/ && \
cd zlib-1.2.11/ && \
./configure && \
make && \
make install

# Sets the actual wroot working directory
WORKDIR /wroot

EXPOSE 8000

# Keep the container running
CMD ["/wroot/bin/wroot"]

