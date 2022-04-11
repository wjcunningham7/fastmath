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
ACLOCAL_AMFLAGS = -I m4 --install

bin_PROGRAMS = general clone count intersection union disjointunion difference hdf5
general_SOURCES = general.cpp
clone_SOURCES = clone.cpp
count_SOURCES = count.cpp
intersection_SOURCES = intersection.cpp
union_SOURCES = union.cpp
disjointunion_SOURCES = disjointunion.cpp
difference_SOURCES = difference.cpp
hdf5_SOURCES = hdf5.cpp

AM_CXXFLAGS = -I $(top_builddir)/../include/fastmath
