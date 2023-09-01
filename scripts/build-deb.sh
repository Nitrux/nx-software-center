#! /bin/bash

set -x

### Update sources

wget -qO /etc/apt/sources.list.d/nitrux-depot.list https://raw.githubusercontent.com/Nitrux/iso-tool/legacy/configs/files/sources/sources.list.nitrux

wget -qO /etc/apt/sources.list.d/nitrux-testing.list https://raw.githubusercontent.com/Nitrux/iso-tool/legacy/configs/files/sources/sources.list.nitrux.testing

curl -L https://packagecloud.io/nitrux/depot/gpgkey | apt-key add -;
curl -L https://packagecloud.io/nitrux/unison/gpgkey | apt-key add -;
curl -L https://packagecloud.io/nitrux/testing/gpgkey | apt-key add -;

apt -qq update

### Update GCC #1
### Use the libappimage package from Mantic because the version in Jammy is too old, and AppImageUpdate fails to compile due to "undefined references."
### And also update GCC and CPP because the version of libappimage in Mantic is compiled against GCC 12.2 and CPP 12.

>> ubuntu-mantic.list printf "%s\n" \
    '#################' \
    '# Ubuntu Mantic #' \
    '################' \
    '' \
    'deb [arch=amd64] http://archive.ubuntu.com/ubuntu mantic main restricted universe multiverse' \
    'deb [arch=amd64] http://archive.ubuntu.com/ubuntu mantic-security main restricted universe multiverse' \
    'deb [arch=amd64] http://archive.ubuntu.com/ubuntu mantic-updates main restricted universe multiverse' \
    ''

mv ubuntu-mantic.list /etc/apt/sources.list.d/

apt -qq update

apt -qq -yy install --no-install-recommends --only-upgrade \
	gcc

apt -qq -yy install --no-install-recommends \
	libappimage-dev

rm -r \
	/etc/apt/sources.list.d/ubuntu-mantic.list

apt -qq update

### Install Package Build Dependencies #2

apt -qq -yy install --no-install-recommends \
	mauikit-accounts-git \
	mauikit-filebrowsing-git \
	mauikit-git

apt -qq -yy Dpkg::Options::="--force-overwrite" install --no-install-recommends \
	libappimageupdate \
	zsync2

rm -rf {LICENSE,README.md}

### Compile Source
### Note: Use curl =< 7.81.0 because libcpr will fail to compile against newer versions of curl due to libcpr using a deprecated API call; see https://github.com/libcpr/cpr/issues/870

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
	-DCMAKE_INSTALL_LIBDIR=/usr/lib/x86_64-linux-gnu ..

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
	--requires="libappimage1.0abi1 \(\>= 1.0.4\),libc6,libgcrypt20,libkf5coreaddons5,libkf5i18n5,libqt5core5a,libqt5gui5,libqt5network5,libqt5qml5,libqt5widgets5,libstdc++6,mauikit-git \(\>= 3.0.1+git\),mauikit-accounts-git \(\>= 3.0.1+git\),mauikit-filebrowsing-git \(\>= 3.0.1+git\),qml-module-qt-labs-platform,qml-module-qtwebview,zsync2,libappimageupdate" \
	--nodoc \
	--strip=no \
	--stripso=yes \
	--reset-uids=yes \
	--deldesc=yes
