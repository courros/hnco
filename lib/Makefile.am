AM_CXXFLAGS = $(OPENMP_CXXFLAGS)

lib_LTLIBRARIES = libhnco.la

libhnco_la_SOURCES = \
	hnco/algorithms/algorithm.cc \
	hnco/algorithms/algorithm.hh \
	hnco/algorithms/all.hh \
	hnco/algorithms/bm-pbil/bm-pbil.cc \
	hnco/algorithms/bm-pbil/bm-pbil.hh \
	hnco/algorithms/bm-pbil/model.cc \
	hnco/algorithms/bm-pbil/model.hh \
	hnco/algorithms/complete-search.cc \
	hnco/algorithms/complete-search.hh \
	hnco/algorithms/decorators/restart.cc \
	hnco/algorithms/decorators/restart.hh \
	hnco/algorithms/ea/all.hh \
	hnco/algorithms/ea/crossover.cc \
	hnco/algorithms/ea/crossover.hh \
	hnco/algorithms/ea/genetic-algorithm.cc \
	hnco/algorithms/ea/genetic-algorithm.hh \
	hnco/algorithms/ea/mu-comma-lambda-ea.cc \
	hnco/algorithms/ea/mu-comma-lambda-ea.hh \
	hnco/algorithms/ea/mu-plus-lambda-ea.cc \
	hnco/algorithms/ea/mu-plus-lambda-ea.hh \
	hnco/algorithms/ea/one-plus-lambda-comma-lambda-ga.cc \
	hnco/algorithms/ea/one-plus-lambda-comma-lambda-ga.hh \
	hnco/algorithms/ea/one-plus-one-ea.hh \
	hnco/algorithms/ea/tournament-selection.cc \
	hnco/algorithms/ea/tournament-selection.hh \
	hnco/algorithms/eda/all.hh \
	hnco/algorithms/eda/mimic.cc \
	hnco/algorithms/eda/mimic.hh \
	hnco/algorithms/fast-efficient-p3/all.hh \
	hnco/algorithms/fast-efficient-p3/hboa.cc \
	hnco/algorithms/fast-efficient-p3/hboa.hh \
	hnco/algorithms/fast-efficient-p3/hnco-evaluator.hh \
	hnco/algorithms/fast-efficient-p3/ltga.cc \
	hnco/algorithms/fast-efficient-p3/ltga.hh \
	hnco/algorithms/fast-efficient-p3/p3.cc \
	hnco/algorithms/fast-efficient-p3/p3.hh \
	hnco/algorithms/fast-efficient-p3/src/Configuration.cpp \
	hnco/algorithms/fast-efficient-p3/src/Configuration.h \
	hnco/algorithms/fast-efficient-p3/src/Evaluation.cpp \
	hnco/algorithms/fast-efficient-p3/src/Evaluation.h \
	hnco/algorithms/fast-efficient-p3/src/HBOA.cpp \
	hnco/algorithms/fast-efficient-p3/src/HBOA.h \
	hnco/algorithms/fast-efficient-p3/src/HillCimb.cpp \
	hnco/algorithms/fast-efficient-p3/src/HillClimb.h \
	hnco/algorithms/fast-efficient-p3/src/LTGA.cpp \
	hnco/algorithms/fast-efficient-p3/src/LTGA.h \
	hnco/algorithms/fast-efficient-p3/src/MiddleLayer.cpp \
	hnco/algorithms/fast-efficient-p3/src/MiddleLayer.h \
	hnco/algorithms/fast-efficient-p3/src/Optimizer.h \
	hnco/algorithms/fast-efficient-p3/src/Population.cpp \
	hnco/algorithms/fast-efficient-p3/src/Population.h \
	hnco/algorithms/fast-efficient-p3/src/Pyramid.cpp \
	hnco/algorithms/fast-efficient-p3/src/Pyramid.h \
	hnco/algorithms/fast-efficient-p3/src/Record.cpp \
	hnco/algorithms/fast-efficient-p3/src/Record.h \
	hnco/algorithms/fast-efficient-p3/src/Util.cpp \
	hnco/algorithms/fast-efficient-p3/src/Util.h \
	hnco/algorithms/hea/all.hh \
	hnco/algorithms/hea/hea.hh \
	hnco/algorithms/hea/herding-binary.cc \
	hnco/algorithms/hea/herding-binary.hh \
	hnco/algorithms/hea/herding-spin.cc \
	hnco/algorithms/hea/herding-spin.hh \
	hnco/algorithms/hea/moment-binary.cc \
	hnco/algorithms/hea/moment-binary.hh \
	hnco/algorithms/hea/moment-spin.cc \
	hnco/algorithms/hea/moment-spin.hh \
	hnco/algorithms/ls/all.hh \
	hnco/algorithms/ls/first-ascent-hill-climbing.cc \
	hnco/algorithms/ls/first-ascent-hill-climbing.hh \
	hnco/algorithms/ls/random-local-search.cc \
	hnco/algorithms/ls/random-local-search.hh \
	hnco/algorithms/ls/random-walk.cc \
	hnco/algorithms/ls/random-walk.hh \
	hnco/algorithms/ls/simulated-annealing.cc \
	hnco/algorithms/ls/simulated-annealing.hh \
	hnco/algorithms/ls/steepest-ascent-hill-climbing.cc \
	hnco/algorithms/ls/steepest-ascent-hill-climbing.hh \
	hnco/algorithms/matrix.hh \
	hnco/algorithms/population.cc \
	hnco/algorithms/population.hh \
	hnco/algorithms/pv/all.hh \
	hnco/algorithms/pv/compact-ga.cc \
	hnco/algorithms/pv/compact-ga.hh \
	hnco/algorithms/pv/mmas.cc \
	hnco/algorithms/pv/mmas.hh \
	hnco/algorithms/pv/nps-pbil.cc \
	hnco/algorithms/pv/nps-pbil.hh \
	hnco/algorithms/pv/pbil.cc \
	hnco/algorithms/pv/pbil.hh \
	hnco/algorithms/pv/probability-vector.cc \
	hnco/algorithms/pv/probability-vector.hh \
	hnco/algorithms/pv/pv-algorithm.cc \
	hnco/algorithms/pv/pv-algorithm.hh \
	hnco/algorithms/pv/umda.cc \
	hnco/algorithms/pv/umda.hh \
	hnco/algorithms/random-search.cc \
	hnco/algorithms/random-search.hh \
	hnco/bit-matrix.cc \
	hnco/bit-matrix.hh \
	hnco/bit-vector.cc \
	hnco/bit-vector.hh \
	hnco/exception.hh \
	hnco/functions/all.hh \
	hnco/functions/cancellation.cc \
	hnco/functions/cancellation.hh \
	hnco/functions/decorators/all.hh \
	hnco/functions/decorators/function-controller.cc \
	hnco/functions/decorators/function-controller.hh \
	hnco/functions/decorators/function-decorator.hh \
	hnco/functions/decorators/function-modifier.cc \
	hnco/functions/decorators/function-modifier.hh \
	hnco/functions/decorators/prior-noise.cc \
	hnco/functions/decorators/prior-noise.hh \
	hnco/functions/equal-products.cc \
	hnco/functions/equal-products.hh \
	hnco/functions/four-peaks.cc \
	hnco/functions/four-peaks.hh \
	hnco/functions/function.cc \
	hnco/functions/function.hh \
	hnco/functions/jump.cc \
	hnco/functions/jump.hh \
	hnco/functions/labs.cc \
	hnco/functions/labs.hh \
	hnco/functions/linear-function.cc \
	hnco/functions/linear-function.hh \
	hnco/functions/long-path.cc \
	hnco/functions/long-path.hh \
	hnco/functions/max-sat.cc \
	hnco/functions/max-sat.hh \
	hnco/functions/nk-landscape.cc \
	hnco/functions/nk-landscape.hh \
	hnco/functions/qubo.cc \
	hnco/functions/qubo.hh \
	hnco/functions/theory.cc \
	hnco/functions/theory.hh \
	hnco/functions/trap.cc \
	hnco/functions/trap.hh \
	hnco/functions/walsh/all.hh \
	hnco/functions/walsh/walsh-expansion-1.cc \
	hnco/functions/walsh/walsh-expansion-1.hh \
	hnco/functions/walsh/walsh-expansion-2.cc \
	hnco/functions/walsh/walsh-expansion-2.hh \
	hnco/functions/walsh/walsh-expansion.cc \
	hnco/functions/walsh/walsh-expansion.hh \
	hnco/iterator.cc \
	hnco/iterator.hh \
	hnco/map.cc \
	hnco/map.hh \
	hnco/neighborhoods/neighborhood-iterator.cc \
	hnco/neighborhoods/neighborhood-iterator.hh \
	hnco/neighborhoods/neighborhood.cc \
	hnco/neighborhoods/neighborhood.hh \
	hnco/permutation.cc \
	hnco/permutation.hh \
	hnco/random.cc \
	hnco/random.hh \
	hnco/sparse-bit-matrix.cc \
	hnco/sparse-bit-matrix.hh \
	hnco/sparse-bit-vector.cc \
	hnco/sparse-bit-vector.hh \
	hnco/util.hh

if ENABLE_FACTORIZATION
libhnco_la_SOURCES += \
	hnco/functions/factorization.cc \
	hnco/functions/factorization.hh
endif

if ENABLE_PLUGIN
libhnco_la_SOURCES += \
	hnco/functions/plugin.cc \
	hnco/functions/plugin.hh
endif

nobase_include_HEADERS = \
	hnco/algorithms/algorithm.hh \
	hnco/algorithms/all.hh \
	hnco/algorithms/bm-pbil/bm-pbil.hh \
	hnco/algorithms/bm-pbil/model.hh \
	hnco/algorithms/complete-search.hh \
	hnco/algorithms/decorators/restart.hh \
	hnco/algorithms/ea/all.hh \
	hnco/algorithms/ea/crossover.hh \
	hnco/algorithms/ea/genetic-algorithm.hh \
	hnco/algorithms/ea/mu-comma-lambda-ea.hh \
	hnco/algorithms/ea/mu-plus-lambda-ea.hh \
	hnco/algorithms/ea/one-plus-lambda-comma-lambda-ga.hh \
	hnco/algorithms/ea/one-plus-one-ea.hh \
	hnco/algorithms/ea/tournament-selection.hh \
	hnco/algorithms/eda/all.hh \
	hnco/algorithms/eda/mimic.hh \
	hnco/algorithms/fast-efficient-p3/all.hh \
	hnco/algorithms/fast-efficient-p3/hboa.hh \
	hnco/algorithms/fast-efficient-p3/hnco-evaluator.hh \
	hnco/algorithms/fast-efficient-p3/ltga.hh \
	hnco/algorithms/fast-efficient-p3/src/Configuration.h \
	hnco/algorithms/fast-efficient-p3/src/Evaluation.h \
	hnco/algorithms/fast-efficient-p3/src/HBOA.h \
	hnco/algorithms/fast-efficient-p3/src/HillClimb.h \
	hnco/algorithms/fast-efficient-p3/src/LTGA.h \
	hnco/algorithms/fast-efficient-p3/src/MiddleLayer.h \
	hnco/algorithms/fast-efficient-p3/src/Optimizer.h \
	hnco/algorithms/fast-efficient-p3/src/Population.h \
	hnco/algorithms/fast-efficient-p3/src/Pyramid.h \
	hnco/algorithms/fast-efficient-p3/src/Record.h \
	hnco/algorithms/fast-efficient-p3/src/Util.h \
	hnco/algorithms/hea/all.hh \
	hnco/algorithms/hea/hea.hh \
	hnco/algorithms/hea/herding-binary.hh \
	hnco/algorithms/hea/herding-spin.hh \
	hnco/algorithms/hea/moment-binary.hh \
	hnco/algorithms/hea/moment-spin.hh \
	hnco/algorithms/ls/all.hh \
	hnco/algorithms/ls/first-ascent-hill-climbing.hh \
	hnco/algorithms/ls/random-local-search.hh \
	hnco/algorithms/ls/random-walk.hh \
	hnco/algorithms/ls/simulated-annealing.hh \
	hnco/algorithms/ls/steepest-ascent-hill-climbing.hh \
	hnco/algorithms/matrix.hh \
	hnco/algorithms/population.hh \
	hnco/algorithms/pv/all.hh \
	hnco/algorithms/pv/compact-ga.hh \
	hnco/algorithms/pv/mmas.hh \
	hnco/algorithms/pv/nps-pbil.hh \
	hnco/algorithms/pv/pbil.hh \
	hnco/algorithms/pv/probability-vector.hh \
	hnco/algorithms/pv/pv-algorithm.hh \
	hnco/algorithms/pv/umda.hh \
	hnco/algorithms/random-search.hh \
	hnco/bit-matrix.hh \
	hnco/bit-vector.hh \
	hnco/exception.hh \
	hnco/functions/all.hh \
	hnco/functions/cancellation.hh \
	hnco/functions/decorators/all.hh \
	hnco/functions/decorators/function-controller.hh \
	hnco/functions/decorators/function-decorator.hh \
	hnco/functions/decorators/function-modifier.hh \
	hnco/functions/decorators/prior-noise.hh \
	hnco/functions/equal-products.hh \
	hnco/functions/four-peaks.hh \
	hnco/functions/function.hh \
	hnco/functions/jump.hh \
	hnco/functions/labs.hh \
	hnco/functions/linear-function.hh \
	hnco/functions/long-path.hh \
	hnco/functions/max-sat.hh \
	hnco/functions/nk-landscape.hh \
	hnco/functions/qubo.hh \
	hnco/functions/theory.hh \
	hnco/functions/trap.hh \
	hnco/functions/walsh/all.hh \
	hnco/functions/walsh/walsh-expansion-1.hh \
	hnco/functions/walsh/walsh-expansion-2.hh \
	hnco/functions/walsh/walsh-expansion.hh \
	hnco/iterator.hh \
	hnco/map.hh \
	hnco/neighborhoods/neighborhood-iterator.hh \
	hnco/neighborhoods/neighborhood.hh \
	hnco/permutation.hh \
	hnco/random.hh \
	hnco/sparse-bit-matrix.hh \
	hnco/sparse-bit-vector.hh \
	hnco/util.hh

if ENABLE_FACTORIZATION
nobase_include_HEADERS += hnco/functions/factorization.hh
endif

if ENABLE_PLUGIN
nobase_include_HEADERS += hnco/functions/plugin.hh
endif

EXTRA_DIST = hnco/algorithms/fast-efficient-p3/src/LICENSE.txt

#
# Code management
#

.PHONY: doc

doc: $(SOURCES) $(BUILT_SOURCES)
	cd $(top_srcdir) && doxygen Doxyfile
