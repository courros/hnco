# Copyright (C) 2016, 2017, 2018, 2019, 2020 Arnaud Berny

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

SUBDIRS = lib app tests examples experiments

ACLOCAL_AMFLAGS = -I m4

EXTRA_DIST = \
	Doxyfile \
	README.md

#
# Code management
#

.PHONY: doc count count-hnco count-all

doc:
	cd lib/ && make doc

count:
	cd $(top_srcdir) && (find . -name "*.hh" -or -name "*.cc" -or -name "*.h" -or -name "*.cpp" | xargs wc -l)

count-hnco:
	cloc --no3 --by-file-by-lang $(top_srcdir)/lib/hnco $(top_srcdir)/scripts $(top_srcdir)/app $(top_srcdir)/tests

count-all:
	cloc --no3 --by-file-by-lang $(top_srcdir)/lib $(top_srcdir)/scripts $(top_srcdir)/app $(top_srcdir)/tests
