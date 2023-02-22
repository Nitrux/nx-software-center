#! /bin/bash

set -x

### Update sources

wget -qO /etc/apt/sources.list.d/nitrux-depot.list https://raw.githubusercontent.com/Nitrux/iso-tool/legacy/configs/files/sources/sources.list.nitrux

wget -qO /etc/apt/sources.list.d/nitrux-testing.list https://raw.githubusercontent.com/Nitrux/iso-tool/legacy/configs/files/sources/sources.list.nitrux.testing

curl -L https://packagecloud.io/nitrux/depot/gpgkey | apt-key add -;
curl -L https://packagecloud.io/nitrux/unison/gpgkey | apt-key add -;
curl -L https://packagecloud.io/nitrux/testing/gpgkey | apt-key add -;

apt -qq update

### Install Package Build Dependencies #2
### NX Software Center needs ECM > 5.70
### NX Software Center needs CMake > 3.19

apt -qq -yy install --no-install-recommends \
	mauikit-accounts-git \
	mauikit-filebrowsing-git \
	mauikit-git \
    zsync2 \
	libappimage-dev/trixie

rm -rf {LICENSE,README.md}

### Compile Source

mkdir -p build && cd build

cmake \
	-DCMAKE_INSTALL_PREFIX=/usr \
	-DENABLE_BSYMBOLICFUNCTIONS=OFF \
	-DQUICK_COMPILER=ON \
	-D_GLIBCXX_USE_CXX11_ABI=0 \
	-DCMAKE_BUILD_TYPE=Release \
	-DCMAKE_INSTALL_SYSCONFDIR=/etc \
	-DCMAKE_INSTALL_LOCALSTATEDIR=/var \
	-DCMAKE_EXPORT_NO_PACKAGE_REGISTRY=ON \
	-DCMAKE_FIND_PACKAGE_NO_PACKAGE_REGISTRY=ON \
	-DCMAKE_INSTALL_RUNSTATEDIR=/run "-GUnix Makefiles" \
	-DCMAKE_VERBOSE_MAKEFILE=ON \
	-DCMAKE_INSTALL_LIBDIR=lib/x86_64-linux-gnu ..

make -j$(nproc)

make install

### Run checkinstall and Build Debian Package

>> description-pak printf "%s\n" \
	'Software store for AppImages built with MauiKit.' \
	'' \
	'Software Center for Nitrux.' \
	'' \
	''

checkinstall -D -y \
	--install=no \
	--fstrans=yes \
	--pkgname=nx-software-center-git \
	--pkgversion=$PACKAGE_VERSION \
	--pkgarch=amd64 \
	--pkgrelease="1" \
	--pkglicense=LGPL-3 \
	--pkggroup=utils \
	--pkgsource=nx-software-center \
	--pakdir=. \
	--maintainer=uri_herrera@nxos.org \
	--provides=nx-software-center \
	--requires="libappimage1.0abi1 \(\>= 1.0.4\),libc6,libgcc-s1,libgcrypt20,libkf5coreaddons5,libkf5i18n5,libqt5core5a,libqt5gui5,libqt5network5,libqt5qml5,libqt5widgets5,libstdc++6,mauikit-git \(\>= 2.2.2+git\),mauikit-accounts-git \(\>= 2.2.2+git\),mauikit-filebrowsing-git \(\>= 2.2.2+git\),qml-module-qt-labs-platform,qml-module-qtwebview,zsync2,libappimageupdate" \
	--nodoc \
	--strip=no \
	--stripso=yes \
	--reset-uids=yes \
	--deldesc=yes
