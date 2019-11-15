# Created and maintained by: Bruno dos Santos Bermann
# All rights reserved.

# Get the base Ubuntu image from Docker Hub
FROM ubuntu:18.04

# Install the development environment
# WARNING: Based on "wroot/dev/install-dev.sh", if you change this file, please also update install-dev.sh.

# Sets the install wroot working directory temporarily
WORKDIR /tmp/wroot/install

RUN apt-get update && apt-get install -y \
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
psmisc \
valgrind \
kcachegrind

# CMake 3.15+
RUN wget -O - https://apt.kitware.com/keys/kitware-archive-latest.asc 2>/dev/null | apt-key add - && \
apt-add-repository 'deb https://apt.kitware.com/ubuntu/ bionic main' && \
apt-get update && \
apt-get install -y cmake

# GCC 6.3.0 (wroot build) + 4.8 (for building old libs)
RUN add-apt-repository ppa:ubuntu-toolchain-r/test -y && \
apt-get update && \
apt-get install -y gcc-9 g++-9 libc6-dev gdb gdbserver

RUN update-alternatives --install /usr/bin/gcc gcc /usr/bin/gcc-9 90 --slave /usr/bin/g++ g++ /usr/bin/g++-9

# BOOST
RUN add-apt-repository ppa:mhier/libboost-latest -y && \
apt-get update && \
apt-get install -y libboost1.70-dev

# Lua
RUN apt-get update && \
apt-get install -y luarocks

# Lua Mongo
RUN wget https://github.com/mongodb/mongo-c-driver/releases/download/1.15.2/mongo-c-driver-1.15.2.tar.gz && \
tar -xvzf mongo-c-driver-1.15.2.tar.gz && \
cd mongo-c-driver-1.15.2/ && \
cmake . && \
make install && \
luarocks install lua-mongo

# SSH (Doesn't need to be put in install-dev.sh since it is only useful for remote debug on docker)
RUN apt-get update && apt-get install -y openssh-server rsync

RUN mkdir /var/run/sshd
RUN echo 'root:wroot' | chpasswd
RUN sed -i 's/#PermitRootLogin prohibit-password/PermitRootLogin yes/' /etc/ssh/sshd_config

RUN apt-get update && apt-get install -y python3 python3-pip
RUN pip3 install cpplint

RUN apt-get update && \
apt-get install -y php7.2-fpm php7.2-mbstring php7.2-xml php7.2-zip zip unzip

# Sets the actual wroot working directory
WORKDIR /wroot

# wRoot
EXPOSE 8000
EXPOSE 666
# SSH
EXPOSE 22
# GDB Server
EXPOSE 7777

CMD /usr/sbin/sshd -D && bash
