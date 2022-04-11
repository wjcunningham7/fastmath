Name:		fastmath
Version:	1.0.0
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
./configure 

%install
make install

%changelog
* Sun Apr 11 2022 Will Cunningham <wjcunningham7@gmail.com> - 0.0.1
- First package version
