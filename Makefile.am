AUTOMAKE_OPTIONS = foreign
ACLOCAL_AMFLAGS = -I m4 --install

SUBDIRS = nint src

lib_LIBRARIES = libfastmath.a
include_HEADERS = inc/config.h inc/fastapprox.h inc/FastBitset.h inc/FastMath.h inc/FastNumInt.h inc/stopwatch.h nint/nint.hpp @xtra@
libfastmath_a_LIBADD = nint/nint.o src/FastMath.o src/FastNumInt.o src/stopwatch.o
libfastmath_a_SOURCES = 
EXTRA_HEADERS =
