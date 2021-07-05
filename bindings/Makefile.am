# Copyright (C) 2016, 2017, 2018, 2019, 2020, 2021 Arnaud Berny

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

AM_CPPFLAGS = -I $(top_srcdir)/lib $(BOOST_CPPFLAGS) $(shell python3 -m pybind11 --includes)

AM_CXXFLAGS = $(CXXFLAGS) $(OPENMP_CXXFLAGS)

AM_LDFLAGS = $(BOOST_LDFLAGS)

AM_LIBS = ../lib/.libs/libhnco.so $(LIBS) $(BOOST_SERIALIZATION_LIB)

MODULE_NAME = hnco$(shell python3-config --extension-suffix)

#
# Python module
#

ALL_TARGETS =

if ENABLE_BINDINGS
    ALL_TARGETS += $(MODULE_NAME)
endif

all: $(ALL_TARGETS)

$(MODULE_NAME): $(top_srcdir)/bindings/hnco.cc
	$(CXX) $(AM_CPPFLAGS) $(AM_CXXFLAGS) -shared -fPIC $(top_srcdir)/bindings/hnco.cc -o $(MODULE_NAME) $(AM_LDFLAGS) $(AM_LIBS)
