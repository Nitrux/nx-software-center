#!/usr/bin/env bash

APPDIR=$1
if [ -z "${APPDIR}" ]; then
    echo "Missing AppDir path!"
    echo "Example:
        bash appimage.include.icon.theme.sh ./AppDir"
    exit 1
fi


mkdir -p ${APPDIR}/etc/xdg/
cp /etc/xdg/kdeglobals ${APPDIR}/etc/xdg/nx_software_centerrc
