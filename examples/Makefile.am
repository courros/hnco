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

noinst_PROGRAMS = \
	basic \
	cli-my-algorithm \
	cli-my-function \
	cli-universal-function-adapter \
	custom \
	on-budget-function \
	stop-on-maximum

AM_CXXFLAGS = $(OPENMP_CXXFLAGS)

AM_CPPFLAGS = -I $(top_srcdir)/lib

LDADD = ../lib/libhnco.la -ldl -lboost_serialization

basic_SOURCES = basic.cc
cli_my_algorithm_SOURCES = cli-my-algorithm.cc
cli_my_function_SOURCES = cli-my-function.cc
cli_universal_function_adapter_SOURCES = cli-universal-function-adapter.cc
stop_on_maximum_SOURCES = stop-on-maximum.cc
on_budget_function_SOURCES = on-budget-function.cc
custom_SOURCES = custom.cc

.PHONY: plugin

plugin:
	gcc -fPIC -c $(top_srcdir)/examples/onemax.c
	gcc -shared -Wl,-soname,libfoo.so -o libfoo.so onemax.o
