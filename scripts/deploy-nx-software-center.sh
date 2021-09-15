#!/bin/bash

git clone https://github.com/Nitrux/nx-software-center.git

cd nx-software-center
cmake -DCMAKE_BUILD_TYPE="Release" \
    -DCMAKE_INSTALL_PREFIX="/usr" \
    .

make -j$(nproc)
make install