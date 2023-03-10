#!/bin/bash

# Build and Install AppImageUpdate from repository
# git clone https://github.com/AppImage/AppImageUpdate.git --recursive --depth=1 -b fix/cmake_pkg_depends

# cd AppImageUpdate
# cmake -DCMAKE_BUILD_TYPE="Release" \
#     -DCMAKE_INSTALL_PREFIX="/usr" \
#     .

# make -j$(nproc)
# DESTDIR=/ make install
# make install


# Install appimageupdate from Nitrux repo since building it is giving some errors now.
wget -qO /etc/apt/sources.list.d/nitrux-depot.list https://raw.githubusercontent.com/Nitrux/iso-tool/legacy/configs/files/sources/sources.list.nitrux
wget -qO /etc/apt/sources.list.d/nitrux-testing.list https://raw.githubusercontent.com/Nitrux/iso-tool/legacy/configs/files/sources/sources.list.nitrux.testing
curl -L https://packagecloud.io/nitrux/depot/gpgkey | apt-key add -;
curl -L https://packagecloud.io/nitrux/unison/gpgkey | apt-key add -;
curl -L https://packagecloud.io/nitrux/testing/gpgkey | apt-key add -;

sudo apt-get update -q

apt -qq -yy install --no-install-recommends \
  libappimageupdate zsync2

rm -r \
  /etc/apt/sources.list.d/nitrux-depot.list

rm -r \
  /etc/apt/sources.list.d/nitrux-testing.list

apt -qq update