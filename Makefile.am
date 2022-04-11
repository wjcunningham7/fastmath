# Copyright 2014-2022 Will Cunningham
#
# This file is part of FastMath.
#
# Licensed under the GNU General Public License 3.0 (the "License").
# A copy of the License may be obtained with this software package or at
#
#     https://www.gnu.org/licenses/gpl-3.0.en.html
#
# Use of this file is prohibited except in compliance with the License. Any
# modifications or derivative works of this file must retain this copyright
# notice, and modified files must contain a notice indicating that they have
# been altered from the originals.
#
# FastMath is distributed in the hope that it will be useful, but WITHOUT 
# ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or 
# FITNESS FOR A PARTICULAR PURPOSE. See the License for more details.

AUTOMAKE_OPTIONS = foreign
ACLOCAL_AMFLAGS = --install

sourcedir = include/fastmath
pkginclude_HEADERS = \
	$(sourcedir)/config.h \
	$(sourcedir)/fastapprox.h \
	$(sourcedir)/fastbitset.h \
	$(sourcedir)/fastmath.h \
	$(sourcedir)/integration.h \
	$(sourcedir)/mersenne.h \
	$(sourcedir)/nint.h \
	$(sourcedir)/printcolor.h \
	$(sourcedir)/progressbar.h \
	$(sourcedir)/stopwatch.h \
	$(sourcedir)/resources.h

distclean-local:
	rm -rf $(top_builddir)/{\
autom4te.cache,\
aclocal.m4,\
AUTHORS,\
ChangeLog,\
configure,\
configure~,\
install-sh,\
Makefile.in,\
missing,\
NEWS,\
$(sourcedir)/config.h.in,\
$(sourcedir)/config.h.in~\
}
