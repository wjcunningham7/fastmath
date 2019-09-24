###########################
#(C) Will Cunningham 2017 #
#         DK Lab          #
# Northeastern University #
###########################

AUTOMAKE_OPTIONS = foreign
ACLOCAL_AMFLAGS = -I m4 --install

bin_PROGRAMS = general clone count intersection union disjointunion difference
general_SOURCES = general.cpp
clone_SOURCES = clone.cpp
count_SOURCES = count.cpp
intersection_SOURCES = intersection.cpp
union_SOURCES = union.cpp
disjointunion_SOURCES = disjointunion.cpp
difference_SOURCES = difference.cpp

AM_CXXFLAGS = -I $(top_builddir) -I $(top_builddir)/inc
