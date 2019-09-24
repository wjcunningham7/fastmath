###########################
#(C) Will Cunningham 2017 #
#         DK Lab          #
# Northeastern University #
###########################

AUTOMAKE_OPTIONS = foreign
ACLOCAL_AMFLAGS = -I m4 --install

noinst_LIBRARIES = libfast.a
libfast_a_SOURCES = printcolor.cpp progressbar.cpp stopwatch.cpp fastmath.cpp nint.cpp integration.cpp resources.cpp
AM_CXXFLAGS = -I $(top_builddir) -I $(top_builddir)/inc
