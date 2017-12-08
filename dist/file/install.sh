#!/bin/bash

echo "Installing wRoot..."

sudo cp libmysqlclient.so.18 /usr/lib/
sudo cp libmysqlcppconn.so.7 /usr/lib/

sudo apt-get install -y php7.1

echo "Done."