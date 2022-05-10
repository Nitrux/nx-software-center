#!/bin/bash

if [ "$EUID" -ne 0 ]; then
    APT_COMMAND="sudo apt-get"
else
    APT_COMMAND="apt-get"
fi

$APT_COMMAND update -q
$APT_COMMAND install -qy --no-install-recommends \
    build-essential \
    cmake \
    curl \
    git \
    gtk-update-icon-cache \
    extra-cmake-modules \
    qtbase5-dev \
    qtdeclarative5-dev \
    qtmultimedia5-dev \
    qtquickcontrols2-5-dev \
    libkf5coreaddons-dev \
    libkf5i18n-dev \
    gettext \
    libappimage-dev \
    libqt5svg5-dev \
    libkf5config-dev \
    libkf5notifications-dev \
    qtpositioning5-dev \
    libexiv2-dev \
    kquickimageeditor-dev \
    libkf5kio-dev \
    libqt5x11extras5-dev \
    libxcb-shape0-dev \
    libxcb-icccm4-dev \
    libssl-dev \
    automake \
    libtool \
    xxd \
    desktop-file-utils \
    libfuse-dev \
    libcurl4-openssl-dev \
    libkf5completion-dev \
    argagg-dev \
    libgcrypt20-dev \
    libkf5service-dev \
    libkf5bookmarks-dev \
    libkf5widgetsaddons-dev \
    libkf5itemviews-dev \
    libkf5jobwidgets-dev \
    libkf5solid-dev \
    libkf5xmlgui-dev \
    libkf5configwidgets-dev \
    libkf5auth-dev \
    libkf5codecs-dev \
    libkf5windowsystem-dev