# NX Software Center [![Build Status](https://travis-ci.org/Nitrux/nx-software-center.svg?branch=dev-mauikit-ui)](https://travis-ci.org/Nitrux/nx-software-center)

_A graphical front end to manage [AppImage](https://appimage.org/) applications built using [MauiKit](mauikit.org/)._

The AppImages displayed are fetched from:
* https://www.appimagehub.com/

# Screenshots
 
![](https://user-images.githubusercontent.com/5911329/99120975-c80ecf00-25c9-11eb-8712-7a7c14d3b928.png)

# Install

### Requirements
 * CMake 3.10.0 +
 * Qt 5.10+
 * KF5 Plasma 5.45.0 +
 * LibAppImage (latest)
 
### Build
 1. `git clone https://github.com/Nitrux/nx-software-center.git` 
 1. `mkdir build`
 1. `cd build`
 1. `cmake ../nx-software-center -DCMAKE_BUILD_PREFIX=Release -DCMAKE_INSTALL_PREFIX=/usr`
 1. `make`

# Issues
If you find problems with the contents of this repository please create an issue.

©2021 Nitrux Latinoamericana S.C.
