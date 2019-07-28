#!/bin/bash

cd ../
wroot_dir=$PWD

echo Creating wroot directory...
mkdir bin > /dev/null 2>&1 && cp -R dist/cfg/* bin/

echo Copying wroot binaries...
cp -R dist/bin/* bin/

echo Moving Scripts...
cp -R dist/scripts bin/;
chmod 777 bin/scripts -R;

cd src/3rdParty/lua
make

cd ${wroot_dir}
cd bin

cmake -DCMAKE_BUILD_TYPE=Debug ../
make
