AUTOMAKE_OPTIONS = foreign
ACLOCAL_AMFLAGS = -I m4 --install

SUBDIRS = nint src inc

lib_LIBRARIES = libfastmath.a
libfastmath_a_LIBADD = nint/nint.o src/FastMath.o src/FastNumInt.o src/stopwatch.o
libfastmath_a_SOURCES = 
