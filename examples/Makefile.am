bin_PROGRAMS = \
	one-plus-one-ea-one-max

AM_CXXFLAGS = $(OPENMP_CXXFLAGS)

AM_CPPFLAGS = -I $(top_srcdir)/lib

LDADD = ../lib/libhnco.la -ldl -lboost_serialization

one_plus_one_ea_one_max_SOURCES = one-plus-one-ea-one-max.cc
