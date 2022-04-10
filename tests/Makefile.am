# Copyright 2014-2022 Will Cunningham
#
# This file is part of FastMath.
#
# Licensed under the MIT License (the "License"). A copy of the
# License may be obtained with this software package or at
#
#     https://opensource.org/licenses/MIT
#
# FastMath is distributed in the hope that it will be useful, but
# WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

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

AM_CXXFLAGS = -I $(top_builddir) -I $(top_builddir)/inc
