AUTOMAKE_OPTIONS = foreign
ACLOCAL_AMFLAGS = -I m4 --install

noinst_LIBRARIES = libnint.a
libnint_a_SOURCES = nint.cpp

cleanup:
	@rm -rf autom4te.cache .deps aclocal.m4 config.guess config.hpp.in* config.log config.status config.sub depcomp install-sh libtool ltmain.sh missing nint.o stamp-h1

cleanall: maintainer-clean-am
	@rm -rf autom4te.cache .deps aclocal.m4 config.guess config.hpp.* config.log config.status config.sub configure depcomp install-sh ltmain.sh Makefile Makefile.in missing
