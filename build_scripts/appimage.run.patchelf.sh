#!/usr/bin/env bash

pushd appdir

patchelf --set-rpath '$ORIGIN/../../lib/' ./usr/qml/QtQuick.2/libqtquick2plugin.so

popd