#!/bin/bash

set -eu -o pipefail

PKG_ROOT=`dirname $0`
BUILD=$PKG_ROOT/build
mkdir -p $BUILD/{BUILD,RPMS,SOURCES,SPECS,SRPMS}

VERSION=`cat $PKG_ROOT/VERSION`

tar cvzf fastmath-$VERSION.tar.gz $PKG_ROOT/{include,Makefile.am,configure.ac,LICENSE}
mv fastmath-$VERSION.tar.gz $BUILD/SOURCES

cat <<-EOF > $BUILD/SPECS/fastmath.spec
Name:		fastmath
Version:	$VERSION
Release:	1
Summary:	Numerical approximations and compact data structures for fast math operations.

License:	GPLv3
URL: 		https://github.com/wjcunningham7/%{name}
Source0:	%{name}-%{version}.tar.gz

BuildArch:	x86_64

BuildRequires:	make

Requires:	boost >= 1.55.0
Requires:	gsl-devel >= 1.13

%build
touch NEWS AUTHORS ChangeLog
autoreconf -vfi
./configure $@

%install
make install

%changelog
* Sun Apr 11 2022 Will Cunningham <wjcunningham7@gmail.com> - 0.0.1
- First package version
EOF
