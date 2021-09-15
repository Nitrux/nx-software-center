#!/bin/bash

#MAUIKIT_IMAGETOOLS_VERSION=v2.0

git clone https://invent.kde.org/maui/mauikit-imagetools.git --depth=1

cd mauikit-imagetools
cmake -DCMAKE_BUILD_TYPE="Release" \
    -DCMAKE_INSTALL_PREFIX="/usr" \
    .

make -j$(nproc)
DESTDIR=/ make install
make install