#!/usr/bin/env bash

build_scripts_dir=`dirname $0`
source_dir=`dirname $build_scripts_dir`

./$build_scripts_dir/appimage.create.inital.appdir.sh || { echo "AppImage Creation Failed" && exit 1; }

./$build_scripts_dir/appimage.create.icon.sh || { echo "AppImage Creation Failed" && exit 1; }

./$build_scripts_dir/appimage.include.qml.modules.py "$source_dir/src/qml/" appdir $QML_MODULES_FIND_DIRS || { echo "AppImage Creation Failed" && exit 1; }

./$build_scripts_dir/appimage.include.plasma.theme.sh nomad || { echo "AppImage Creation Failed" && exit 1; }

./$build_scripts_dir/appimage.include.icon.theme.sh Luv || { echo "AppImage Creation Failed" && exit 1; }

./$build_scripts_dir/appimage.create.wrapper.sh || { echo "AppImage Creation Failed" && exit 1; }

./$build_scripts_dir/appimage.include.kde.configs.sh || { echo "AppImage Creation Failed" && exit 1; }

./$build_scripts_dir/appimage.run.patchelf.sh || { echo "AppImage Creation Failed" && exit 1; }

./$build_scripts_dir/appimage.run.linuxdeployqt.sh || { echo "AppImage Creation Failed" && exit 1; }

./$build_scripts_dir/appimage.run.appimagetool.sh || { echo "AppImage Creation Failed" && exit 1; }
