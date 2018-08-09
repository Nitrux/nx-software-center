#!/usr/bin/env bash

mkdir -p tools
pushd tools
wget -N https://github.com/linuxdeploy/linuxdeploy/releases/download/continuous/linuxdeploy-"$ARCH".AppImage || { echo "linuxdeploy download Failed" && exit 1; }
chmod +x linuxdeploy-"$ARCH".AppImage

wget -N https://github.com/linuxdeploy/linuxdeploy-plugin-qt/releases/download/continuous/linuxdeploy-plugin-qt-"$ARCH".AppImage || { echo "linuxdeploy-plugin-qt download Failed" && exit 1; }
chmod +x linuxdeploy-plugin-qt-"$ARCH".AppImage
popd

export LINUXDEPLOY_BIN=${PWD}/tools/linuxdeploy-"$ARCH".AppImage
export LINUXDEPLOY_PLUGIN_QT_BIN=${PWD}/tools/linuxdeploy-plugin-qt-"$ARCH".AppImage
