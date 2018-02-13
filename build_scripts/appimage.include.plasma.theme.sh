#!/usr/bin/env bash


if [[ $# != 1 ]]; then
    echo "Missing theme name!"
    echo "Example:
    bash appimage.include.plasma.theme.sh my_theme"
    exit 1
fi

BASE_THEMES_PATH="/usr/share/plasma/desktoptheme"
THEME_PATH="${BASE_THEMES_PATH}/${1}"

echo "Including plasma theme"
if [ -d ${THEME_PATH} ]; then
    TARGET_THEMES_DIR="./appdir${BASE_THEMES_PATH}"
    mkdir -p ${TARGET_THEMES_DIR}
    cp -L -r ${BASE_THEMES_PATH}/default ${TARGET_THEMES_DIR}
    cp -L -r ${THEME_PATH} ${TARGET_THEMES_DIR}
else
    echo "ERROR: Theme \"$1\" not found."
    exit 1
fi