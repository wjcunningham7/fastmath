###########################
#(C) Will Cunningham 2017 #
#         DK Lab          #
# Northeastern University #
###########################

AUTOMAKE_OPTIONS = foreign
ACLOCAL_AMFLAGS = -I m4 --install

SUBDIRS = nint src inc test

lib_LIBRARIES = libfastmath.a
libfastmath_a_LIBADD = nint/nint.o src/FastMath.o src/FastNumInt.o src/stopwatch.o src/progressbar.o
libfastmath_a_SOURCES = 

distclean-local:
	rm -rf $(top_builddir)/{autom4te.cache,aclocal.m4,AUTHORS,ChangeLog,config.guess,config.log,config.status,config.sub,configure,depcomp,install-sh,Makefile,Makefile.in,missing,NEWS,.deps,inc/config.h*,inc/Makefile.in,src/.dirstamp,src/Makefile.in,nint/Makefile.in,test/Makefile.in,compile}
