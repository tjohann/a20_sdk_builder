#!/bin/bash

sudo make uninstall
make distclean

autoreconf --install || exit 1
./configure --prefix=$HOME

cd po
make update-po

cd ..
make
sudo make install

a20_sdk_builder &
