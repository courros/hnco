# Copyright (C) 2016, 2017, 2018, 2019, 2020, 2021, 2022, 2023, 2024 Arnaud Berny

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

SUBDIRS = lib app tests examples experiments bindings

ACLOCAL_AMFLAGS = -I m4

EXTRA_DIST = \
	Doxyfile \
	README.md \
	docs/Doxyfile \
	docs/Makefile \
	docs/acknowledgments.rst \
	docs/building.rst \
	docs/commandline.rst \
	docs/conf.py \
	docs/contributing.rst \
	docs/experiments.rst \
	docs/index.rst \
	docs/introduction.rst \
	docs/library.rst \
	docs/name.rst \
	docs/python.rst \
	docs/references.rst \
	docs/requirements.rst \
	tools/optgen.pl

#
# Code management
#

.PHONY: doc count count-hnco count-all

doc:
	cd $(top_srcdir) && doxygen

count:
	cd $(top_srcdir) && (find . -name "*.hh" -or -name "*.cc" -or -name "*.h" -or -name "*.cpp" | xargs wc -l)

count-hnco:
	cloc --no3 --by-file-by-lang $(top_srcdir)/lib/hnco $(top_srcdir)/experiments $(top_srcdir)/app $(top_srcdir)/tests $(top_srcdir)/bindings

count-all:
	cloc --no3 --by-file-by-lang $(top_srcdir)/lib $(top_srcdir)/experiments $(top_srcdir)/app $(top_srcdir)/tests $(top_srcdir)/bindings

#
# Bash completion scripts
#

bashcompdir = @bashcompdir@
dist_bashcomp_DATA = \
	$(top_srcdir)/app/ffgen.bash \
	$(top_srcdir)/app/mapgen.bash \
	$(top_srcdir)/lib/hnco/app/hnco.bash \
	$(top_srcdir)/lib/hnco/multiobjective/app/hnco-mo.bash
