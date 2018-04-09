#!/usr/bin/env bash


which patchelf || { echo "ERROR: patchelf not found!" && exit 1; }

pushd appdir

patchelf --set-rpath '$ORIGIN/../../lib/' ./usr/qml/QtQuick.2/libqtquick2plugin.so

popd