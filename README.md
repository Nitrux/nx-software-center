# NX Software Center [![Build Status](https://travis-ci.org/Nitrux/nx-software-center.svg?branch=development)](https://travis-ci.org/Nitrux/nx-software-center)

_A portable Software Center for portable applications thanks to [AppImage](https://appimage.org/)._

You can now easily explore and get your favorite applications easily (in some cases directly from their original authors!). No matter which GNU/Linux distribution you are using.

Say goodbye to things like:
 * The applications that I want is not in my distribution repositories.
 * Missing or outdated libraries and package conflicts.
 * Old application version.

**Just download and run!**

## Feeds
The AppImages included are fetched from:
* https://appimage.github.io/
* https://www.appimagehub.com/

# Screenshots

 * Search View
 
![](https://user-images.githubusercontent.com/1138094/39208689-58d3ae84-47c9-11e8-83a9-28d89a6f2cbb.png)

 * Application Details View
 
![](https://user-images.githubusercontent.com/1138094/39208724-6d128f5a-47c9-11e8-90e3-d41472374406.png)

 * Tasks View
 
![](https://user-images.githubusercontent.com/1138094/39208748-7978ded4-47c9-11e8-98c4-5dffeaf5ee0d.png)

## Build
### Requirements
 * CMake 3.10.0 +
 * Qt 5.10+
 * KF5 Plasma 5.45.0 +
 * LibAppImage (latest)
 
### Instructions
 1. `git clone https://github.com/Nitrux/nx-software-center.git` 
 1. `mkdir build`
 1. `cd build`
 1. `cmake ../nx-software-center -DCMAKE_BUILD_PREFIX=Release -DCMAKE_INSTALL_PREFIX=/usr`
 1. `make`

# Issues
If you find problems with the contents of this repository please create an issue.

©2018 Nitrux Latinoamericana S.C.
