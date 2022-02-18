# Copyright (C) 2016, 2017, 2018, 2019, 2020, 2021, 2022 Arnaud Berny

# This file is part of HNCO.

# HNCO is free software: you can redistribute it and/or modify it
# under the terms of the GNU Lesser General Public License as
# published by the Free Software Foundation, either version 3 of the
# License, or (at your option) any later version.

# HNCO is distributed in the hope that it will be useful, but WITHOUT
# ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
# or FITNESS FOR A PARTICULAR PURPOSE. See the GNU Lesser General
# Public License for more details.

# You should have received a copy of the GNU Lesser General Public
# License along with HNCO. If not, see
# <http://www.gnu.org/licenses/>.

bin_PROGRAMS = \
	ffgen \
	hnco \
	hnco-mo \
	mapgen

BUILT_SOURCES = \
	ffgen-options.cc \
	ffgen-options.hh \
	mapgen-options.cc \
	mapgen-options.hh

EXTRA_DIST = \
	ffgen.bash \
	mapgen.bash

AM_CPPFLAGS = -I $(top_srcdir)/lib

AM_CXXFLAGS = $(OPENMP_CXXFLAGS)

LDADD = ../lib/libhnco.la $(BOOST_SERIALIZATION_LIB)

#
# ffgen program
#

ffgen_SOURCES = \
	ffgen.json \
	ffgen-options.cc \
	ffgen-options.hh \
	ffgen.cc

ffgen-options.hh ffgen-options.cc: ffgen.json
	cd $(top_srcdir)/app && $(top_srcdir)/tools/optgen.pl ffgen.json

#
# hnco program
#

hnco_SOURCES = hnco.cc

#
# hnco-mo program
#

hnco_mo_SOURCES = hnco-mo.cc

#
# mapgen program
#

mapgen_SOURCES = \
	mapgen.json \
	mapgen-options.cc \
	mapgen-options.hh \
	mapgen.cc

mapgen-options.hh mapgen-options.cc: mapgen.json
	cd $(top_srcdir)/app && $(top_srcdir)/tools/optgen.pl mapgen.json
