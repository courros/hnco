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

SUBDIRS = lib app tests examples

ACLOCAL_AMFLAGS = -I m4

EXTRA_DIST = \
	Doxyfile \
	README.md \
	experiments/examples/autocorrelation/Makefile \
	experiments/examples/autocorrelation/instances/Makefile \
	experiments/examples/autocorrelation/instances/ep.100 \
	experiments/examples/autocorrelation/instances/lin.100 \
	experiments/examples/autocorrelation/instances/ms.100.3.1000 \
	experiments/examples/autocorrelation/instances/nk.100.4 \
	experiments/examples/autocorrelation/instances/walsh2.100 \
	experiments/examples/autocorrelation/plan.json \
	experiments/examples/autocorrelation/report/Makefile \
	experiments/examples/autocorrelation/report/report.tex \
	experiments/examples/benchmark/Makefile \
	experiments/examples/benchmark/instances/Makefile \
	experiments/examples/benchmark/instances/ep.100 \
	experiments/examples/benchmark/instances/lin.100 \
	experiments/examples/benchmark/instances/ms.100.3.1000 \
	experiments/examples/benchmark/instances/nk.100.4 \
	experiments/examples/benchmark/instances/walsh2.100 \
	experiments/examples/benchmark/plan.json \
	experiments/examples/benchmark/report/Makefile \
	experiments/examples/benchmark/report/report.tex \
	experiments/examples/dynamics/Makefile \
	experiments/examples/dynamics/instances/Makefile \
	experiments/examples/dynamics/instances/ep.100 \
	experiments/examples/dynamics/instances/lin.100 \
	experiments/examples/dynamics/instances/ms.100.3.1000 \
	experiments/examples/dynamics/instances/nk.100.4 \
	experiments/examples/dynamics/instances/walsh2.100 \
	experiments/examples/dynamics/plan.json \
	experiments/examples/dynamics/report/Makefile \
	experiments/examples/dynamics/report/report.tex \
	experiments/examples/ecdf/Makefile \
	experiments/examples/ecdf/instances/Makefile \
	experiments/examples/ecdf/instances/ep.100 \
	experiments/examples/ecdf/instances/lin.100 \
	experiments/examples/ecdf/instances/ms.100.3.1000 \
	experiments/examples/ecdf/instances/nk.100.4 \
	experiments/examples/ecdf/instances/walsh2.100 \
	experiments/examples/ecdf/plan.json \
	experiments/examples/ecdf/report/Makefile \
	experiments/examples/ecdf/report/report.tex \
	experiments/examples/function-parameter/Makefile \
	experiments/examples/function-parameter/plan.json \
	experiments/examples/function-parameter/report/Makefile \
	experiments/examples/function-parameter/report/report.tex \
	experiments/examples/maximum/Makefile \
	experiments/examples/maximum/instances/Makefile \
	experiments/examples/maximum/instances/ep.100 \
	experiments/examples/maximum/instances/lin.100 \
	experiments/examples/maximum/instances/ms.100.3.1000 \
	experiments/examples/maximum/instances/nk.100.4 \
	experiments/examples/maximum/instances/walsh2.100 \
	experiments/examples/maximum/plan.json \
	experiments/examples/maximum/report/Makefile \
	experiments/examples/maximum/report/report.tex \
	experiments/examples/parameter/Makefile \
	experiments/examples/parameter/instances/Makefile \
	experiments/examples/parameter/instances/ep.100 \
	experiments/examples/parameter/instances/lin.100 \
	experiments/examples/parameter/instances/ms.100.3.1000 \
	experiments/examples/parameter/instances/nk.100.4 \
	experiments/examples/parameter/instances/walsh2.100 \
	experiments/examples/parameter/plan.json \
	experiments/examples/parameter/report/Makefile \
	experiments/examples/parameter/report/report.tex \
	experiments/examples/runtime/Makefile \
	experiments/examples/runtime/plan.json \
	experiments/examples/runtime/report/Makefile \
	experiments/examples/runtime/report/report.tex \
	experiments/examples/walsh-transforms/Makefile \
	experiments/examples/walsh-transforms/instances/Makefile \
	experiments/examples/walsh-transforms/instances/ep.10 \
	experiments/examples/walsh-transforms/instances/lin.10 \
	experiments/examples/walsh-transforms/instances/ms.10.3.10 \
	experiments/examples/walsh-transforms/instances/nk.10.2 \
	experiments/examples/walsh-transforms/instances/walsh2.10 \
	experiments/examples/walsh-transforms/plan.json \
	experiments/examples/walsh-transforms/report/Makefile \
	experiments/examples/walsh-transforms/report/report.tex

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
