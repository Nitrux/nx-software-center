#!/usr/bin/env bash

echo "## Install into appdir"
make DESTDIR="appdir" -j$(nproc) install
find appdir/

echo "Getting linuxdeployqt"
wget -c -nv "https://github.com/probonopd/linuxdeployqt/releases/download/continuous/linuxdeployqt-continuous-x86_64.AppImage"
chmod a+x linuxdeployqt-continuous-x86_64.AppImage

# Create a png icon for the AppImage
wget -c -nv "https://raw.githubusercontent.com/nomad-desktop/luv-icon-theme/master/Luv/apps/64/nx-software-center.svg"
inkscape -z -e appdir/nx-software-center.png -w 256 -h 256 nx-software-center.svg

bash "`dirname $0`/appimage.include.qml.modules.sh" .. appdir

# Create the appimage
unset QTDIR; unset QT_PLUGIN_PATH ; unset LD_LIBRARY_PATH
export VERSION=$(git rev-parse --short HEAD)

./linuxdeployqt-continuous-x86_64.AppImage \
    appdir/usr/share/applications/*.desktop \
    -qmldir=/usr/lib/x86_64-linux-gnu/qt5/qml \
    -bundle-non-qt-libs \
    -appimage

ls -lh
find appdir/
