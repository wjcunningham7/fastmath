Name:		fastmath
Version:	1.3.2
Release:	1
Summary:	FastMath Toolkit

License:	GPLv3
URL: 		https://github.com/wjcunningham7/fastmath
Source0:	fastmath-1.3.2.tar.gz

BuildArch:	x86_64

BuildRequires:	make

Requires:	boost
Requires:	gsl-devel >= 1.13

%description
The FastMath toolkit provides efficient numerical approximations and compact data structures

%prep
%setup -q
touch NEWS AUTHORS ChangeLog
autoreconf -vfi

%build
%configure

%install
%makeinstall

%files
%defattr(-,root,root,-)
/usr/include/fastmath/config.h
/usr/include/fastmath/fastapprox.h
/usr/include/fastmath/fastbitset.h
/usr/include/fastmath/fastmath.h
/usr/include/fastmath/integration.h
/usr/include/fastmath/mersenne.h
/usr/include/fastmath/nint.h
/usr/include/fastmath/printcolor.h
/usr/include/fastmath/progressbar.h
/usr/include/fastmath/resources.h
/usr/include/fastmath/stopwatch.h

%changelog
* Sun Apr 11 2022 Will Cunningham <wjcunningham7@gmail.com> - 0.0.1
- First package version
