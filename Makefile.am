AUTOMAKE_OPTIONS = subdir-objects

ACLOCAL_AMFLAGS = -I m4 --install
SUBDIRS = opt/nintlib

noinst_LIBRARIES = $(top_builddir)/lib/libfastmath.a
__top_builddir__lib_libfastmath_a_SOURCES = src/stopwatch.cpp src/FastMath.cpp src/FastNumInt.cpp
__top_builddir__lib_libfastmath_a_LIBADD = opt/nintlib/nint.o
# AM_CXX_FLAGS = -I inc -I opt

cleanup:
	@rm -rf autom4te.cache .deps aclocal.m4 config.guess inc/config.h.* config.log config.status config.sub depcomp install-sh libtool ltmain.sh missing opt/nintlib/nint.o opt/nintlib/.deps stopwatch.o FastMath.o FastNumInt.o stamp-h1 COPYING INSTALL

cleanall: maintainer-clean-am
	@rm -rf autom4te.cache .deps aclocal.m4 config.guess inc/config.h.* config.log config.status config.sub configure depcomp install-sh ltmain.sh Makefile Makefile.in missing lib AUTHORS ChangeLog NEWS COPYING INSTALL
