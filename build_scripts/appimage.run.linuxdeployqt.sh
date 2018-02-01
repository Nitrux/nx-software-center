#!/usr/bin/env bash

echo "Getting linuxdeployqt"
wget -c -nv "https://github.com/probonopd/linuxdeployqt/releases/download/continuous/linuxdeployqt-continuous-x86_64.AppImage"
chmod a+x linuxdeployqt-continuous-x86_64.AppImage

# Create the appimage
unset QTDIR; unset QT_PLUGIN_PATH ; unset LD_LIBRARY_PATH
export VERSION=$(git rev-parse --short HEAD)

./linuxdeployqt-continuous-x86_64.AppImage \
    appdir/usr/share/applications/*.desktop \
    -qmldir=/usr/lib/x86_64-linux-gnu/qt5/qml \
    -bundle-non-qt-libs \
    -appimage

exit $?