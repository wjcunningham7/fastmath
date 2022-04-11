#!/bin/bash

set -eu -o pipefail

PKG_ROOT=`dirname $0`
VERSION=`cat $PKG_ROOT/VERSION`

cat <<-EOF > fastmath.spec
Name:		fastmath
Version:	$VERSION
Release:	1
Summary:	FastMath Toolkit

License:	GPLv3
URL: 		https://github.com/wjcunningham7/fastmath

BuildArch:	x86_64

BuildRequires:	make

Requires:	boost >= 1.55.0
Requires:	gsl-devel >= 1.13

%description
The FastMath toolkit provides efficient numerical approximations and compact data structures

%build
%configure --prefix=/usr/local

%install
%makeinstall

%changelog
* Sun Apr 11 2022 Will Cunningham <wjcunningham7@gmail.com> - 0.0.1
- First package version
EOF
