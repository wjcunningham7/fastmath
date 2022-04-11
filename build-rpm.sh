#!/bin/bash

set -eu -o pipefail

PKG_ROOT=`dirname $0`
VERSION=`cat $PKG_ROOT/VERSION`
BUILD=$PKG_ROOT/build
mkdir -p $BUILD/{BUILD,RPMS,SOURCES,SPECS,SRPMS}

tar cvzf fastmath-$VERSION.tar.gz $PKG_ROOT/{\
include,\
configure,\
LICENSE\
}

mv fastmath-$VERSION.tar.gz $BUILD/SOURCES

cat <<-EOF > $BUILD/SPECS/fastmath.spec
Name:		fastmath
Version:	$VERSION
Release:	1
Summary:	FastMath Toolkit

License:	GPLv3
URL: 		https://github.com/wjcunningham7/fastmath
Source0:	fastmath-$VERSION.tar.gz

BuildArch:	x86_64

BuildRequires:	make

Requires:	boost >= 1.55.0
Requires:	gsl-devel >= 1.13

%description
The FastMath toolkit provides efficient numerical approximations and compact data structures

%prep
%setup -q

%build
%configure --prefix=/usr/local

%install
%makeinstall

%changelog
* Sun Apr 11 2022 Will Cunningham <wjcunningham7@gmail.com> - 0.0.1
- First package version
EOF
