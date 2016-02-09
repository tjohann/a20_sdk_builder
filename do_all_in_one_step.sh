#!/bin/bash

sudo make uninstall
make distclean

autoreconf --install || exit 1
./configure --prefix=$HOME --sysconfdir=/etc

cd po
make update-po

cd ..
make
sudo make install

sdk_builder -f a20_sdk_builder.conf &
