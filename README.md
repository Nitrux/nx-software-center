# NX Software Center

_A graphical front end to manage [AppImage](https://appimage.org/) applications built using [MauiKit](https://mauikit.org/)._

The AppImages displayed are fetched from:
* https://www.appimagehub.com/

# Screenshots
 
![](https://user-images.githubusercontent.com/5911329/99120975-c80ecf00-25c9-11eb-8712-7a7c14d3b928.png)

# Install

### Requirements

#### Debian/Ubuntu

```
 extra-cmake-modules,
 libkf5config-dev (>= 5.24.0~),
 libkf5coreaddons-dev (>= 5.24.0~),
 libkf5i18n-dev (>= 5.24.0~),
 libkf5kio-dev,
 libkf5notifications-dev (>= 5.24.0~),
 libkf5service-dev (>= 5.24.0~),
 libqt5svg5-dev,
 libqt5webview5-dev,
 libqt5xmlpatterns5-dev,
 libfuse-dev,
 libappimage-dev,
 desktop-file-utils,
 mauikit-dev,
 libmauikitfilebrowsing-dev,
 libmauikitaccounts-dev,
 libappimage-dev,
 qt5-qmake,
 qtbase5-dev (>= 5.5.0~),
 qtdeclarative5-dev,
 qtquickcontrols2-5-dev,
 qtwebengine5-dev
```

### Build
 1. `git clone https://github.com/Nitrux/nx-software-center.git` 
 1. `mkdir build`
 1. `cd build`
 1. `cmake ../nx-software-center -DCMAKE_BUILD_PREFIX=Release -DCMAKE_INSTALL_PREFIX=/usr`
 1. `make`

# Issues
If you find problems with the contents of this repository please create an issue.

©2021 Nitrux Latinoamericana S.C.
