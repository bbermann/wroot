#!/bin/sh

echo Creating wroot directory...
mkdir bin > /dev/null 2>&1 && cp -R dist/cfg/* bin/

echo Copying wroot binaries...
cp -R dist/bin/* bin/

echo Moving Scripts...
cp -R dist/scripts bin/;
sudo chmod 777 bin/scripts -R;

cd bin

qmake ../wroot.pro
make
