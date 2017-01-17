AUTOMAKE_OPTIONS = foreign
ACLOCAL_AMFLAGS = -I m4 --install

bin_PROGRAMS = general clone count intersection union disjointunion difference coords
general_SOURCES = general.cpp
clone_SOURCES = clone.cpp
count_SOURCES = count.cpp
intersection_SOURCES = intersection.cpp
union_SOURCES = union.cpp
disjointunion_SOURCES = disjointunion.cpp
difference_SOURCES = difference.cpp
coords_SOURCES = coords.cpp

AM_CXXFLAGS = -I $(top_builddir) -I $(top_builddir)/inc
