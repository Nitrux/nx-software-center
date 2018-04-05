#!/usr/bin/env bash

set -x

echo "Getting linuxdeployqt"
wget -c -nv "https://github.com/probonopd/linuxdeployqt/releases/download/continuous/linuxdeployqt-continuous-x86_64.AppImage" || { echo "ERROR: Unable to get linuxdeployqt!" && exit 1; }
chmod a+x linuxdeployqt-continuous-x86_64.AppImage

# Create the appimage
unset QTDIR; unset QT_PLUGIN_PATH ; unset LD_LIBRARY_PATH
export VERSION=$(git rev-parse --short HEAD)

env | grep LD_
LINUX_DEPLOY_QT_EXCLUDE_COPYRIGHTS=true ./linuxdeployqt-continuous-x86_64.AppImage \
    appdir/usr/share/applications/*.desktop \
    -verbose=1 \
    -qmldir=src/qml \
    -bundle-non-qt-libs || { echo "ERROR: linuxdeployqt failed!" && exit 1; }

pushd appdir
rm AppRun
ln -s usr/bin/nx_software_center_wrapper.sh AppRun
popd