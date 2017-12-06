#!/bin/sh

sudo apt-get update

sudo apt-get install -y build-essential
sudo apt-get install -y libc6-dev libncursesw5-dev
sudo apt-get install -y gdb gdbserver
sudo apt-get install -y qt5-default