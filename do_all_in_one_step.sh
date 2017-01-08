#!/bin/bash

export PKG_CONFIG_PATH=/usr/local/lib/pkgconfig:${PKG_CONFIG_PATH}

sudo make uninstall
make distclean

autoreconf --install || exit 1
#./configure --enable-debug CC=clang --prefix=$HOME --sysconfdir=/etc
./configure --enable-debug --prefix=$HOME --sysconfdir=/etc

cd po
make update-po

cd ..
make
sudo make install

# start conf daemon
# xterm -fn "-misc-fixed-medium-r-normal--14-*-*-*-*-*-iso8859-15" -geometry 120x40 -hold -e "sdk_confd -f a20_sdk_builder.conf" &

# start gui
xterm -fn "-misc-fixed-medium-r-normal--14-*-*-*-*-*-iso8859-15" -geometry 120x40 -hold -e "sdk_builder -f a20_sdk_builder.conf" &

