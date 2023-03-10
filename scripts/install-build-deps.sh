#!/bin/bash

if [ "$EUID" -ne 0 ]; then
    APT_COMMAND="sudo apt-get"
else
    APT_COMMAND="apt-get"
fi

$APT_COMMAND update -q
$APT_COMMAND install -qy --no-install-recommends \
    argagg-dev \
    automake \
    build-essential \
    checkinstall \
    cmake \
    curl \
    desktop-file-utils \
    extra-cmake-modules \
    gettext \
    git \
    gtk-update-icon-cache \
    kquickimageeditor-dev \
    libappimage \
    libboost-program-options-dev \
    libboost-system-dev \
    libboost-test-dev \
    libboost-thread-dev \
    libcurl4-openssl-dev \
    libexiv2-dev \
    libfuse-dev \
    libgcrypt20-dev \
    libgpgme-dev \
    libkf5auth-dev \
    libkf5bookmarks-dev \
    libkf5codecs-dev \
    libkf5completion-dev \
    libkf5config-dev \
    libkf5configwidgets-dev \
    libkf5coreaddons-dev \
    libkf5i18n-dev \
    libkf5itemviews-dev \
    libkf5jobwidgets-dev \
    libkf5kio-dev \
    libkf5notifications-dev \
    libkf5service-dev \
    libkf5solid-dev \
    libkf5widgetsaddons-dev \
    libkf5windowsystem-dev \
    libkf5xmlgui-dev \
    libqt5svg5-dev \
    libqt5x11extras5-dev \
    libssl-dev \
    libtool \
    libxcb-icccm4-dev \
    libxcb-shape0-dev \
    nlohmann-json3-dev \
    qtbase5-dev \
    qtdeclarative5-dev \
    qtmultimedia5-dev \
    qtpositioning5-dev \
    qtquickcontrols2-5-dev \
    xxd \
    libcairo2-dev \
    librsvg2-dev \
    qtsystems5-dev
