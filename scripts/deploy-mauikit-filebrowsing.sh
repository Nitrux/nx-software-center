#!/bin/bash

#MAUIKIT_FILEBROWSING_VERSION=v2.0

git clone https://invent.kde.org/maui/mauikit-filebrowsing.git --depth=1

cd mauikit-filebrowsing
cmake -DCMAKE_BUILD_TYPE="Release" \
    -DCMAKE_INSTALL_PREFIX="/usr" \
    .

make -j$(nproc)
DESTDIR=/ make install
make install