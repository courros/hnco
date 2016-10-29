bin_PROGRAMS = \
	hnco \
	ffgen \
	mapgen

BUILT_SOURCES = \
	hnco-options.cc \
	hnco-options.hh \
	ffgen-options.cc \
	ffgen-options.hh \
	mapgen-options.cc \
	mapgen-options.hh

EXTRA_DIST = \
	hnco.sh \
	ffgen.sh \
	mapgen.sh

AM_CPPFLAGS = -I $(top_srcdir)/lib

LDADD = ../lib/libhnco.la -ldl -lboost_serialization

#
# hnco program
#

hnco_SOURCES = \
	hnco.json \
	hnco-options.cc \
	hnco-options.hh \
	hnco.cc \
	make-algorithm.cc \
	make-algorithm.hh \
	make-function.cc \
	make-function.hh

hnco-options.hh hnco-options.cc: hnco.json
	cd $(top_srcdir)/src && optgen.pl hnco.json

#
# ffgen program
#

ffgen_SOURCES = \
	ffgen.json \
	ffgen-options.cc \
	ffgen-options.hh \
	ffgen.cc

ffgen-options.hh ffgen-options.cc: ffgen.json
	cd $(top_srcdir)/src && optgen.pl ffgen.json

#
# mapgen program
#

mapgen_SOURCES = \
	mapgen.json \
	mapgen-options.cc \
	mapgen-options.hh \
	mapgen.cc

mapgen-options.hh mapgen-options.cc: mapgen.json
	cd $(top_srcdir)/src && optgen.pl mapgen.json
