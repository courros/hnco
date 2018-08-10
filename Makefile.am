SUBDIRS = lib app tests scripts examples

ACLOCAL_AMFLAGS = -I m4

EXTRA_DIST = \
	experiments/autocorrelation/Makefile \
	experiments/autocorrelation/instances/Makefile \
	experiments/autocorrelation/instances/ep.100 \
	experiments/autocorrelation/instances/lin.100 \
	experiments/autocorrelation/instances/ms.100.3.1000 \
	experiments/autocorrelation/instances/nk.100.4 \
	experiments/autocorrelation/instances/walsh2.100 \
	experiments/autocorrelation/plan.json \
	experiments/autocorrelation/report/Makefile \
	experiments/autocorrelation/report/report.tex \
	experiments/benchmark/Makefile \
	experiments/benchmark/instances/Makefile \
	experiments/benchmark/instances/ep.100 \
	experiments/benchmark/instances/lin.100 \
	experiments/benchmark/instances/ms.100.3.1000 \
	experiments/benchmark/instances/nk.100.4 \
	experiments/benchmark/instances/walsh2.100 \
	experiments/benchmark/plan.json \
	experiments/benchmark/report/Makefile \
	experiments/benchmark/report/report.tex \
	experiments/dynamics/Makefile \
	experiments/dynamics/instances/Makefile \
	experiments/dynamics/instances/ep.100 \
	experiments/dynamics/instances/lin.100 \
	experiments/dynamics/instances/ms.100.3.1000 \
	experiments/dynamics/instances/nk.100.4 \
	experiments/dynamics/instances/walsh2.100 \
	experiments/dynamics/plan.json \
	experiments/dynamics/report/Makefile \
	experiments/dynamics/report/report.tex \
	experiments/ecdf/Makefile \
	experiments/ecdf/instances/Makefile \
	experiments/ecdf/instances/ep.100 \
	experiments/ecdf/instances/lin.100 \
	experiments/ecdf/instances/ms.100.3.1000 \
	experiments/ecdf/instances/nk.100.4 \
	experiments/ecdf/instances/walsh2.100 \
	experiments/ecdf/plan.json \
	experiments/ecdf/report/Makefile \
	experiments/ecdf/report/report.tex \
	experiments/maximum/Makefile \
	experiments/maximum/instances/Makefile \
	experiments/maximum/instances/ep.100 \
	experiments/maximum/instances/lin.100 \
	experiments/maximum/instances/ms.100.3.1000 \
	experiments/maximum/instances/nk.100.4 \
	experiments/maximum/instances/walsh2.100 \
	experiments/maximum/plan.json \
	experiments/maximum/report/Makefile \
	experiments/maximum/report/report.tex \
	experiments/parameter/Makefile \
	experiments/parameter/instances/Makefile \
	experiments/parameter/instances/ep.100 \
	experiments/parameter/instances/lin.100 \
	experiments/parameter/instances/ms.100.3.1000 \
	experiments/parameter/instances/nk.100.4 \
	experiments/parameter/instances/walsh2.100 \
	experiments/parameter/plan.json \
	experiments/parameter/report/Makefile \
	experiments/parameter/report/report.tex \
	experiments/runtime/Makefile \
	experiments/runtime/plan.json \
	experiments/runtime/report/Makefile \
	experiments/runtime/report/report.tex \
	experiments/walsh-transforms/Makefile \
	experiments/walsh-transforms/instances/Makefile \
	experiments/walsh-transforms/instances/ep.10 \
	experiments/walsh-transforms/instances/lin.10 \
	experiments/walsh-transforms/instances/ms.10.3.10 \
	experiments/walsh-transforms/instances/nk.10.2 \
	experiments/walsh-transforms/instances/walsh2.10 \
	experiments/walsh-transforms/plan.json \
	experiments/walsh-transforms/report/Makefile \
	experiments/walsh-transforms/report/report.tex

#
# Code management
#

.PHONY: doc count

doc:
	cd lib/ && make doc

count:
	cd $(top_srcdir) && (find . -name "*.hh" -or -name "*.cc" -or -name "*.h" -or -name "*.cpp" | xargs wc -l)
	cloc --no3 --by-file-by-lang $(top_srcdir)/lib $(top_srcdir)/scripts $(top_srcdir)/app $(top_srcdir)/tests
