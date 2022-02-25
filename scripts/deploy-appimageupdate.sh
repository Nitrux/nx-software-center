#!/bin/bash

git clone https://github.com/AppImage/AppImageUpdate.git --recursive --depth=1 -b fix/cmake_pkg_depends

cd AppImageUpdate
cmake -DCMAKE_BUILD_TYPE="Release" \
    -DCMAKE_INSTALL_PREFIX="/usr" \
    .

make -j$(nproc)
DESTDIR=/ make install
make install