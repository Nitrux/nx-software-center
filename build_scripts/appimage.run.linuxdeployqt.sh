#!/usr/bin/env bash

echo "Getting linuxdeployqt"
wget -c -nv "https://github.com/azubieta/linuxdeployqt/releases/download/continuous/linuxdeployqt-continuous-x86_64.AppImage"
chmod a+x linuxdeployqt-continuous-x86_64.AppImage

# Create the appimage
unset QTDIR; unset QT_PLUGIN_PATH ; unset LD_LIBRARY_PATH
export VERSION=$(git rev-parse --short HEAD)

LINUX_DEPLOY_QT_EXCLUDE_COPYRIGHTS=true ./linuxdeployqt-continuous-x86_64.AppImage \
    appdir/usr/share/applications/*.desktop \
    -qmldir=src/qml \
    -bundle-non-qt-libs \
    -appimage

exit $?