#!/usr/bin/env bash

APPDIR=$1
THEME=$2

if [ -z "${APPDIR}" ]; then
    echo "Missing AppDir path!"
    echo "Example:
        bash appimage.include.plasma.theme.sh ./AppDir my_theme"
    exit 1
fi

if [ -z "${THEME}" ]; then
    echo "Missing theme name!"
    echo "Example:
        bash appimage.include.plasma.theme.sh ./AppDir my_theme"
    exit 1
fi

BASE_THEMES_PATH="/usr/share/plasma/desktoptheme/"
THEME_PATH="${BASE_THEMES_PATH}/${THEME}"

echo "Including plasma theme"
if [ -d ${THEME_PATH} ]; then
    TARGET_THEMES_DIR="./appdir${BASE_THEMES_PATH}"
    mkdir -p ${TARGET_THEMES_DIR}
    cp -L -r ${BASE_THEMES_PATH}/default ${TARGET_THEMES_DIR}
    cp -L -r ${THEME_PATH} ${TARGET_THEMES_DIR}
else
    echo "ERROR: Theme \"$THEME\" not found."
    exit 1
fi