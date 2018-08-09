#!/usr/bin/env bash

echo "## Install into ${APP_DIR}"
mkdir ${BUILD_DIR}
pushd build

cmake .. -DCMAKE_BUILD_TYPE=RelWithDebInfo -DCMAKE_INSTALL_PREFIX=/usr || exit 1
DESTDIR=${APP_DIR} make -j$(nproc) install || exit 1

popd
