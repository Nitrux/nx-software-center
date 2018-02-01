#!/usr/bin/env bash

build_scripts_dir=`dirname $0`

./$build_scripts_dir/appimage.create.inital.appdir.sh

./$build_scripts_dir/appimage.create.icon.sh

./$build_scripts_dir/appimage.include.qml.modules.sh .. appdir

./$build_scripts_dir/appimage.run.linuxdeployqt.sh
