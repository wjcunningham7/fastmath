###########################
#(C) Will Cunningham 2017 #
#         DK Lab          #
# Northeastern University #
###########################

AUTOMAKE_OPTIONS = foreign
ACLOCAL_AMFLAGS = -I m4 --install

SUBDIRS = src inc test

lib_LIBRARIES = libfastmath@PLATFORM@.a
libfastmath@PLATFORM@_a_LIBADD = src/nint.o src/printcolor.o src/fastmath.o src/integration.o src/stopwatch.o src/progressbar.o src/resources.o
libfastmath@PLATFORM@_a_SOURCES = 

distclean-local:
	rm -rf $(top_builddir)/{autom4te.cache,aclocal.m4,AUTHORS,ChangeLog,config.guess,config.log,config.status,config.sub,configure,depcomp,install-sh,Makefile,Makefile.in,missing,NEWS,.deps,inc/config.h*,inc/Makefile.in,src/.dirstamp,src/Makefile.in,test/Makefile.in,compile,fastmath}
