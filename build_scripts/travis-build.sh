#!/usr/bin/env bash

wget -qO - http://archive.neon.kde.org/public.key | apt-key add -
echo "deb http://archive.neon.kde.org/user/ xenial main" | tee /etc/apt/sources.list.d/neon.list
wget -qO - http://repo.nxos.org/public.key | apt-key add -
echo "deb http://repo.nxos.org nxos main" | tee /etc/apt/sources.list.d/nxos.list

apt-get update -qq

apt-get -y install --allow-unauthenticated curl inkscape git cmake patchelf extra-cmake-modules qtdeclarative5-dev-tools qml-module-qtgraphicaleffects plasma-framework-dev qtbase5-dev qtdeclarative5-dev libqt5xmlpatterns5-dev nomad-plasma-look-and-feel luv-icon-theme nomad-desktop-settings

cmake -DCMAKE_BUILD_TYPE=release -DCMAKE_INSTALL_PREFIX=/usr
make -j$(nproc)
build_scripts/appimage.create.sh
