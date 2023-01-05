#!/bin/bash

git clone https://github.com/Nitrux/luv-icon-theme.git --depth=1

cd luv-icon-theme
cp -vrf Luv/* "$TARGET_APPDIR"/usr/share/icons/hicolor/
rm "$TARGET_APPDIR"/usr/share/icons/hicolor/icon-theme.cache
