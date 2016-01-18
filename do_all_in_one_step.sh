#!/bin/bash

make uninstall
make distclean

autoreconf --install
./configure --prefix=$HOME

cd po
make update-po

cd ..
make
make install

a20_sdk_builder &
