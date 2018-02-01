#!/usr/bin/env bash

# Create a png icon for the AppImage
wget -c -nv "https://raw.githubusercontent.com/nomad-desktop/luv-icon-theme/master/Luv/apps/64/nx-software-center.svg"
inkscape -z -e appdir/nx-software-center.png -w 256 -h 256 nx-software-center.svg