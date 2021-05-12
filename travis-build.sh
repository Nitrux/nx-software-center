#!/bin/bash

set -x

apt -qq update
apt -qq -yy install equivs curl git wget gnupg2

### FIXME - the container mauikit/ubuntu-18.04-amd64 does have the neon repo but for some idiotic reason it isn't working here

wget -qO /etc/apt/sources.list.d/neon-unstable-repo.list https://raw.githubusercontent.com/Nitrux/iso-tool/development/configs/files/sources.list.neon.unstable

apt-key adv --keyserver keyserver.ubuntu.com --recv-keys \
	55751E5D \
	1B69B2DA > /dev/null

apt -qq update

### Install Dependencies

DEBIAN_FRONTEND=noninteractive apt -qq -yy install devscripts lintian build-essential automake autotools-dev rename
mk-build-deps -i -t "apt-get --yes" -r

### Build Deb

mkdir source
mv ./* source/ # Hack for debuild
cd source
debuild -b -uc -us
