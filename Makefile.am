SUBDIRS = lib src tests scripts examples

ACLOCAL_AMFLAGS = -I m4

EXTRA_DIST = \
	experiments/benchmark/Makefile \
	experiments/benchmark/instances/Makefile \
	experiments/benchmark/instances/ep.100 \
	experiments/benchmark/instances/lin.100 \
	experiments/benchmark/instances/ms.100.3.1000 \
	experiments/benchmark/instances/nk.100.4 \
	experiments/benchmark/instances/quad.100 \
	experiments/benchmark/plan.json \
	experiments/benchmark/report/Makefile \
	experiments/benchmark/report/report.tex \
	experiments/dynamics/Makefile \
	experiments/dynamics/instances/Makefile \
	experiments/dynamics/instances/ep.100 \
	experiments/dynamics/instances/lin.100 \
	experiments/dynamics/instances/ms.100.3.1000 \
	experiments/dynamics/instances/nk.100.4 \
	experiments/dynamics/instances/quad.100 \
	experiments/dynamics/plan.json \
	experiments/dynamics/report/Makefile \
	experiments/dynamics/report/report.tex \
	experiments/parameter/Makefile \
	experiments/parameter/instances/Makefile \
	experiments/parameter/instances/ep.100 \
	experiments/parameter/instances/lin.100 \
	experiments/parameter/instances/ms.100.3.1000 \
	experiments/parameter/instances/nk.100.4 \
	experiments/parameter/instances/quad.100 \
	experiments/parameter/plan.json \
	experiments/parameter/report/Makefile \
	experiments/parameter/report/report.tex \
	experiments/runtime/Makefile \
	experiments/runtime/plan.json \
	experiments/runtime/report/Makefile \
	experiments/runtime/report/report.tex

#
# Code management
#

.PHONY: doc count

doc:
	cd lib/ && make doc

count:
	cd $(top_srcdir) && (find . -name "*.hh" -or -name "*.cc" | xargs wc -l)
	cloc --no3 --by-file-by-lang $(top_srcdir)/lib $(top_srcdir)/scripts $(top_srcdir)/src $(top_srcdir)/tests
