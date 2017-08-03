###########################
#(C) Will Cunningham 2017 #
#         DK Lab          #
# Northeastern University #
###########################

AUTOMAKE_OPTIONS = foreign
ACLOCAL_AMFLAGS = -I m4 --install

noinst_LIBRARIES = libfast.a
libfast_a_SOURCES = stopwatch.cpp FastMath.cpp FastNumInt.cpp
AM_CXXFLAGS = -I $(top_builddir) -I $(top_builddir)/inc -I $(top_builddir)/nint
