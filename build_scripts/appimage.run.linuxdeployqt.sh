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
    -bundle-non-qt-libs

pushd appdir
rm AppRun
ln -s usr/bin/nx_software_center_wrapper.sh AppRun
popd

wget "https://github.com/AppImage/AppImageKit/releases/download/continuous/appimagetool-x86_64.AppImage"
chmod +x appimagetool-x86_64.AppImage
./appimagetool-x86_64.AppImage appdir

exit $?