#!/bin/sh

mkdir bin > /dev/null 2>&1 && cp -R dist/file/* bin/

cp -R dist/bin/* bin/

mkdir bin/scripts > /dev/null 2>&1
cp -R dist/lua bin/scripts/lua

cd bin

qmake ../wRoot.pro
make
