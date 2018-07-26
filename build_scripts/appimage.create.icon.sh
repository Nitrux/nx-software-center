#!/usr/bin/env bash

# Create a png icon for the AppImage
wget -c -nv "https://raw.githubusercontent.com/Nitrux/luv-icon-theme/master/Luv/apps/64/nx-software-center.svg"
rsvg-convert nx-software-center.svg -w 256 -h 256 >> appdir/nx-software-center.png
