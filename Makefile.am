# Copyright 2014-2022 Will Cunningham
#
# This file is part of FastMath.
#
# Licensed under the MIT License (the "License"). A copy of the
# License may be obtained with this software package or at
#
#     https://opensource.org/licenses/MIT
#
# FastMath is distributed in the hope that it will be useful, but
# WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

AUTOMAKE_OPTIONS = foreign
ACLOCAL_AMFLAGS = -I m4 --install

SUBDIRS = src inc test

lib_LIBRARIES = libfastmath@PLATFORM@.a
libfastmath@PLATFORM@_a_LIBADD = src/nint.o src/printcolor.o src/fastmath.o src/integration.o src/stopwatch.o src/progressbar.o src/resources.o
libfastmath@PLATFORM@_a_SOURCES =

distclean-local:
	rm -rf $(top_builddir)/{autom4te.cache,aclocal.m4,AUTHORS,ChangeLog,config.guess,config.log,config.status,config.sub,configure,depcomp,install-sh,Makefile,Makefile.in,missing,NEWS,.deps,inc/config.h*,inc/Makefile.in,src/.dirstamp,src/Makefile.in,test/Makefile.in,compile,fastmath}
