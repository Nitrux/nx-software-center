#!/bin/bash

mkdir -p nx-software-center && cd nx-software-center
cmake -DCMAKE_BUILD_TYPE="Release" \
    -DCMAKE_INSTALL_PREFIX="/usr" \
    ../../

make -j$(nproc)
make install