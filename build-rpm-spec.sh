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

%files
%defattr(-,root,root,-)
/usr/local/include/fastmath/config.h
/usr/local/include/fastmath/fastapprox.h
/usr/local/include/fastmath/fastbitset.h
/usr/local/include/fastmath/fastmath.h
/usr/local/include/fastmath/integration.h
/usr/local/include/fastmath/mersenne.h
/usr/local/include/fastmath/nint.h
/usr/local/include/fastmath/printcolor.h
/usr/local/include/fastmath/progressbar.h
/usr/local/include/fastmath/resources.h
/usr/local/include/fastmath/stopwatch.h

%changelog
* Sun Apr 11 2022 Will Cunningham <wjcunningham7@gmail.com> - 0.0.1
- First package version

