#!/usr/bin/env bash

if [[ $# != 1 ]]; then
    echo "Missing icon theme name!"
    echo "Example:
    bash appimage.include.plasma.theme.sh my_icon_theme"
    exit 1
fi

BASE_THEMES_PATH="/usr/share/icons"
THEME_PATH="${BASE_THEMES_PATH}/${1}"

echo "Including icon theme"
if [ -d ${THEME_PATH} ]; then
    TARGET_THEMES_DIR="./appdir${BASE_THEMES_PATH}"
    mkdir -p ${TARGET_THEMES_DIR}
    cp -L -r ${THEME_PATH} ${TARGET_THEMES_DIR}
else
    echo "ERROR: Theme \"$1\" not found."
    exit 1
fi