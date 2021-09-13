#!/bin/bash

#MAUIKIT_VERSION=v2.0

git clone https://invent.kde.org/maui/mauikit.git --depth=1

cd mauikit
cmake -DCMAKE_BUILD_TYPE="Release" \
    -DCMAKE_INSTALL_PREFIX="/usr" \
    .

make -j$(nproc)
DESTDIR=/ make install
make install