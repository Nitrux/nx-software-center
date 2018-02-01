#!/usr/bin/env bash

echo "## Install into appdir"
make DESTDIR="appdir" -j$(nproc) install
find appdir/
