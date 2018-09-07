#!/usr/bin/env bash

set -x

build_scripts_dir=`dirname $0`
source_dir=`dirname $build_scripts_dir`

export ARCH="x86_64"

source ${build_scripts_dir}/download.linuxdeploy.sh

export BASE_DIR=${PWD}
export BUILD_DIR=${BASE_DIR}/build
export APP_DIR=${BASE_DIR}/AppDir

echo "## Install into ${APP_DIR}"
DESTDIR=${APP_DIR} make -j$(nproc) install || exit 1

APP_NAME=org.nxos.softwarecenter

echo "Building AppImage for ${APP_NAME}"

${build_scripts_dir}/appimage.include.plasma.theme.sh ${APP_DIR} nomad || { echo "AppImage Creation Failed" && exit 1; }

${build_scripts_dir}/appimage.include.icon.theme.sh ${APP_DIR} Luv || { echo "AppImage Creation Failed" && exit 1; }

${build_scripts_dir}/appimage.create.wrapper.sh ${APP_DIR} || { echo "AppImage Creation Failed" && exit 1; }

${build_scripts_dir}/appimage.include.kde.configs.sh ${APP_DIR} || { echo "AppImage Creation Failed" && exit 1; }


${LINUXDEPLOY_BIN} --app-name ${APP_NAME} --appdir ${APP_DIR} --init-appdir || return 1

QML_SOURCES_PATHS="${source_dir}/src" ${LINUXDEPLOY_PLUGIN_QT_BIN} --appdir ${APP_DIR} || return 1

${LINUXDEPLOY_BIN} --app-name ${APP_NAME} --appdir ${APP_DIR} --output appimage || return 1

mkdir ${BASE_DIR}/dist
mv -v Software*AppImage* ${BASE_DIR}/dist || return 1
