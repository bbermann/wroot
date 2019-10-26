#!/bin/bash

wroot_dir="$(dirname $0)"
cd $wroot_dir/../
wroot_dir=$PWD

cd $wroot_dir

echo Creating wroot directory...
mkdir $wroot_dir/bin > /dev/null 2>&1 && \
cp -R $wroot_dir/dist/cfg/* $wroot_dir/bin/

echo Creating wroot document root...
doc_root="$wroot_dir/bin/file"
cp -R -p $wroot_dir/dist/file $doc_root && \
sed -i "s@WROOT_DOCUMENT_ROOT@$doc_root@g" $wroot_dir/bin/wroot.json

echo Copying wroot binaries...
cp -R $wroot_dir/dist/bin/* $wroot_dir/bin/

echo Moving Scripts...
cp -R $wroot_dir/dist/scripts $wroot_dir/bin/
chmod 777 bin/scripts -R

echo Moving Plugins...
cp -R $wroot_dir/dist/plugins $wroot_dir/bin/
chmod 777 bin/plugins -R

cd $wroot_dir/src/3rdParty/LuaJIT && \
make && \
make install

ldconfig

cd ${wroot_dir}/bin

if [[ $1 == "prod" ]];
then
    echo "Using \"Release\" build configuration..."
    cmake -DCMAKE_BUILD_TYPE=Release ../
else
    echo "Using \"Debug\" build configuration..."
    cmakze -DCMAKE_BUILD_TYPE=Debug ../
fi

make
