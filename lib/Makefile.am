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

AM_CXXFLAGS = $(OPENMP_CXXFLAGS)

lib_LTLIBRARIES = libhnco.la

libhnco_la_SOURCES = \
	fast-efficient-p3/Configuration.cpp \
	fast-efficient-p3/Configuration.h \
	fast-efficient-p3/Evaluation.cpp \
	fast-efficient-p3/Evaluation.h \
	fast-efficient-p3/HBOA.cpp \
	fast-efficient-p3/HBOA.h \
	fast-efficient-p3/HillCimb.cpp \
	fast-efficient-p3/HillClimb.h \
	fast-efficient-p3/LTGA.cpp \
	fast-efficient-p3/LTGA.h \
	fast-efficient-p3/MiddleLayer.cpp \
	fast-efficient-p3/MiddleLayer.h \
	fast-efficient-p3/Optimizer.h \
	fast-efficient-p3/Population.cpp \
	fast-efficient-p3/Population.h \
	fast-efficient-p3/Pyramid.cpp \
	fast-efficient-p3/Pyramid.h \
	fast-efficient-p3/Record.cpp \
	fast-efficient-p3/Record.h \
	fast-efficient-p3/Util.cpp \
	fast-efficient-p3/Util.h \
	fparser/extrasrc/fp_identifier_parser.inc \
	fparser/extrasrc/fp_opcode_add.inc \
	fparser/extrasrc/fpaux.hh \
	fparser/extrasrc/fptypes.hh \
	fparser/fparser.cc \
	fparser/fparser.hh \
	fparser/fpconfig.hh \
	fparser/fpoptimizer.cc \
	hnco/algorithms/algorithm.cc \
	hnco/algorithms/algorithm.hh \
	hnco/algorithms/all.hh \
	hnco/algorithms/bm-pbil/bm-pbil.hh \
	hnco/algorithms/bm-pbil/model.cc \
	hnco/algorithms/bm-pbil/model.hh \
	hnco/algorithms/complete-search.cc \
	hnco/algorithms/complete-search.hh \
	hnco/algorithms/decorators/all.hh \
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
	hnco/algorithms/fast-efficient-p3/all.hh \
	hnco/algorithms/fast-efficient-p3/hboa.cc \
	hnco/algorithms/fast-efficient-p3/hboa.hh \
	hnco/algorithms/fast-efficient-p3/hnco-evaluator.hh \
	hnco/algorithms/fast-efficient-p3/implementation.hh \
	hnco/algorithms/fast-efficient-p3/ltga.cc \
	hnco/algorithms/fast-efficient-p3/ltga.hh \
	hnco/algorithms/fast-efficient-p3/p3.cc \
	hnco/algorithms/fast-efficient-p3/p3.hh \
	hnco/algorithms/hea/all.hh \
	hnco/algorithms/hea/hea.hh \
	hnco/algorithms/hea/spin-herding.cc \
	hnco/algorithms/hea/spin-herding.hh \
	hnco/algorithms/hea/spin-moment.cc \
	hnco/algorithms/hea/spin-moment.hh \
	hnco/algorithms/human.cc \
	hnco/algorithms/human.hh \
	hnco/algorithms/iterative-algorithm.cc \
	hnco/algorithms/iterative-algorithm.hh \
	hnco/algorithms/ls/all.hh \
	hnco/algorithms/ls/first-ascent-hill-climbing.cc \
	hnco/algorithms/ls/first-ascent-hill-climbing.hh \
	hnco/algorithms/ls/local-search-algorithm.hh \
	hnco/algorithms/ls/random-local-search.cc \
	hnco/algorithms/ls/random-local-search.hh \
	hnco/algorithms/ls/random-walk.cc \
	hnco/algorithms/ls/random-walk.hh \
	hnco/algorithms/ls/simulated-annealing.cc \
	hnco/algorithms/ls/simulated-annealing.hh \
	hnco/algorithms/ls/steepest-ascent-hill-climbing.cc \
	hnco/algorithms/ls/steepest-ascent-hill-climbing.hh \
	hnco/algorithms/matrix.hh \
	hnco/algorithms/mimic.cc \
	hnco/algorithms/mimic.hh \
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
	hnco/algorithms/random-selection.cc \
	hnco/algorithms/random-selection.hh \
	hnco/algorithms/solution.hh \
	hnco/app/algorithm-factory.cc \
	hnco/app/algorithm-factory.hh \
	hnco/app/application.cc \
	hnco/app/application.hh \
	hnco/app/decorated-function-factory.cc \
	hnco/app/decorated-function-factory.hh \
	hnco/app/function-factory.cc \
	hnco/app/function-factory.hh \
	hnco/app/hnco-options.cc \
	hnco/app/hnco-options.hh \
	hnco/app/hnco.json \
	hnco/bit-matrix.cc \
	hnco/bit-matrix.hh \
	hnco/bit-vector.cc \
	hnco/bit-vector.hh \
	hnco/exception.hh \
	hnco/functions/all.hh \
	hnco/functions/collection/all.hh \
	hnco/functions/collection/cancellation.cc \
	hnco/functions/collection/cancellation.hh \
	hnco/functions/collection/equal-products.cc \
	hnco/functions/collection/equal-products.hh \
	hnco/functions/collection/four-peaks.cc \
	hnco/functions/collection/four-peaks.hh \
	hnco/functions/collection/ising/all.hh \
	hnco/functions/collection/ising/nearest-neighbor-ising-model-1.cc \
	hnco/functions/collection/ising/nearest-neighbor-ising-model-1.hh \
	hnco/functions/collection/ising/nearest-neighbor-ising-model-2.cc \
	hnco/functions/collection/ising/nearest-neighbor-ising-model-2.hh \
	hnco/functions/collection/jump.cc \
	hnco/functions/collection/jump.hh \
	hnco/functions/collection/labs.cc \
	hnco/functions/collection/labs.hh \
	hnco/functions/collection/linear-function.cc \
	hnco/functions/collection/linear-function.hh \
	hnco/functions/collection/long-path.cc \
	hnco/functions/collection/long-path.hh \
	hnco/functions/collection/max-sat.cc \
	hnco/functions/collection/max-sat.hh \
	hnco/functions/collection/nk-landscape.cc \
	hnco/functions/collection/nk-landscape.hh \
	hnco/functions/collection/parsed-multivariate-function.hh \
	hnco/functions/collection/partition.cc \
	hnco/functions/collection/partition.hh \
	hnco/functions/collection/qubo.cc \
	hnco/functions/collection/qubo.hh \
	hnco/functions/collection/sudoku.cc \
	hnco/functions/collection/sudoku.hh \
	hnco/functions/collection/theory.cc \
	hnco/functions/collection/theory.hh \
	hnco/functions/collection/trap.cc \
	hnco/functions/collection/trap.hh \
	hnco/functions/collection/tsp.cc \
	hnco/functions/collection/tsp.hh \
	hnco/functions/collection/walsh/all.hh \
	hnco/functions/collection/walsh/walsh-expansion-1.cc \
	hnco/functions/collection/walsh/walsh-expansion-1.hh \
	hnco/functions/collection/walsh/walsh-expansion-2.cc \
	hnco/functions/collection/walsh/walsh-expansion-2.hh \
	hnco/functions/collection/walsh/walsh-expansion.cc \
	hnco/functions/collection/walsh/walsh-expansion.hh \
	hnco/functions/controllers/all.hh \
	hnco/functions/controllers/controller.cc \
	hnco/functions/controllers/controller.hh \
	hnco/functions/converter.hh \
	hnco/functions/decorator.hh \
	hnco/functions/function.cc \
	hnco/functions/function.hh \
	hnco/functions/modifiers/all.hh \
	hnco/functions/modifiers/modifier.cc \
	hnco/functions/modifiers/modifier.hh \
	hnco/functions/modifiers/parsed-modifier.cc \
	hnco/functions/modifiers/parsed-modifier.hh \
	hnco/functions/modifiers/prior-noise.cc \
	hnco/functions/modifiers/prior-noise.hh \
	hnco/functions/multivariate-function-adapter.hh \
	hnco/functions/permutation-function-adapter.hh \
	hnco/functions/universal-function-adapter.hh \
	hnco/functions/universal-function.hh \
	hnco/functions/walsh-term.hh \
	hnco/iterator.cc \
	hnco/iterator.hh \
	hnco/logging/log-context.hh \
	hnco/logging/logger.cc \
	hnco/logging/logger.hh \
	hnco/maps/map.cc \
	hnco/maps/map.hh \
	hnco/maps/transvection.cc \
	hnco/maps/transvection.hh \
	hnco/neighborhoods/neighborhood-iterator.cc \
	hnco/neighborhoods/neighborhood-iterator.hh \
	hnco/neighborhoods/neighborhood.cc \
	hnco/neighborhoods/neighborhood.hh \
	hnco/permutation.cc \
	hnco/permutation.hh \
	hnco/random.cc \
	hnco/random.hh \
	hnco/representations/all.hh \
	hnco/representations/categorical.hh \
	hnco/representations/complex.hh \
	hnco/representations/float.hh \
	hnco/representations/integer.hh \
	hnco/representations/permutation.hh \
	hnco/serialization.hh \
	hnco/sparse-bit-vector.cc \
	hnco/sparse-bit-vector.hh \
	hnco/stop-watch.hh \
	hnco/util.hh

if ENABLE_FACTORIZATION
libhnco_la_SOURCES += \
	hnco/functions/collection/factorization.cc \
	hnco/functions/collection/factorization.hh
endif

if ENABLE_PLUGIN
libhnco_la_SOURCES += \
	hnco/functions/collection/plugin.cc \
	hnco/functions/collection/plugin.hh
endif

nobase_include_HEADERS = \
	fast-efficient-p3/Configuration.h \
	fast-efficient-p3/Evaluation.h \
	fast-efficient-p3/HBOA.h \
	fast-efficient-p3/HillClimb.h \
	fast-efficient-p3/LTGA.h \
	fast-efficient-p3/MiddleLayer.h \
	fast-efficient-p3/Optimizer.h \
	fast-efficient-p3/Population.h \
	fast-efficient-p3/Pyramid.h \
	fast-efficient-p3/Record.h \
	fast-efficient-p3/Util.h \
	fparser/extrasrc/fp_identifier_parser.inc \
	fparser/extrasrc/fp_opcode_add.inc \
	fparser/extrasrc/fpaux.hh \
	fparser/extrasrc/fptypes.hh \
	fparser/fparser.hh \
	fparser/fpconfig.hh \
	hnco/algorithms/algorithm.hh \
	hnco/algorithms/all.hh \
	hnco/algorithms/bm-pbil/bm-pbil.hh \
	hnco/algorithms/bm-pbil/model.hh \
	hnco/algorithms/complete-search.hh \
	hnco/algorithms/decorators/all.hh \
	hnco/algorithms/decorators/restart.hh \
	hnco/algorithms/ea/all.hh \
	hnco/algorithms/ea/crossover.hh \
	hnco/algorithms/ea/genetic-algorithm.hh \
	hnco/algorithms/ea/mu-comma-lambda-ea.hh \
	hnco/algorithms/ea/mu-plus-lambda-ea.hh \
	hnco/algorithms/ea/one-plus-lambda-comma-lambda-ga.hh \
	hnco/algorithms/ea/one-plus-one-ea.hh \
	hnco/algorithms/fast-efficient-p3/all.hh \
	hnco/algorithms/fast-efficient-p3/hboa.hh \
	hnco/algorithms/fast-efficient-p3/hnco-evaluator.hh \
	hnco/algorithms/fast-efficient-p3/implementation.hh \
	hnco/algorithms/fast-efficient-p3/ltga.hh \
	hnco/algorithms/fast-efficient-p3/p3.hh \
	hnco/algorithms/hea/all.hh \
	hnco/algorithms/hea/hea.hh \
	hnco/algorithms/hea/spin-herding.hh \
	hnco/algorithms/hea/spin-moment.hh \
	hnco/algorithms/human.hh \
	hnco/algorithms/iterative-algorithm.hh \
	hnco/algorithms/ls/all.hh \
	hnco/algorithms/ls/first-ascent-hill-climbing.hh \
	hnco/algorithms/ls/local-search-algorithm.hh \
	hnco/algorithms/ls/random-local-search.hh \
	hnco/algorithms/ls/random-walk.hh \
	hnco/algorithms/ls/simulated-annealing.hh \
	hnco/algorithms/ls/steepest-ascent-hill-climbing.hh \
	hnco/algorithms/matrix.hh \
	hnco/algorithms/mimic.hh \
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
	hnco/algorithms/random-selection.hh \
	hnco/algorithms/solution.hh \
	hnco/app/algorithm-factory.hh \
	hnco/app/application.hh \
	hnco/app/decorated-function-factory.hh \
	hnco/app/function-factory.hh \
	hnco/app/hnco-options.hh \
	hnco/bit-matrix.hh \
	hnco/bit-vector.hh \
	hnco/exception.hh \
	hnco/functions/all.hh \
	hnco/functions/collection/all.hh \
	hnco/functions/collection/cancellation.hh \
	hnco/functions/collection/equal-products.hh \
	hnco/functions/collection/four-peaks.hh \
	hnco/functions/collection/ising/all.hh \
	hnco/functions/collection/ising/nearest-neighbor-ising-model-1.hh \
	hnco/functions/collection/ising/nearest-neighbor-ising-model-2.hh \
	hnco/functions/collection/jump.hh \
	hnco/functions/collection/labs.hh \
	hnco/functions/collection/linear-function.hh \
	hnco/functions/collection/long-path.hh \
	hnco/functions/collection/max-sat.hh \
	hnco/functions/collection/nk-landscape.hh \
	hnco/functions/collection/parsed-multivariate-function.hh \
	hnco/functions/collection/partition.hh \
	hnco/functions/collection/qubo.hh \
	hnco/functions/collection/sudoku.hh \
	hnco/functions/collection/theory.hh \
	hnco/functions/collection/trap.hh \
	hnco/functions/collection/tsp.hh \
	hnco/functions/collection/walsh/all.hh \
	hnco/functions/collection/walsh/walsh-expansion-1.hh \
	hnco/functions/collection/walsh/walsh-expansion-2.hh \
	hnco/functions/collection/walsh/walsh-expansion.hh \
	hnco/functions/controllers/all.hh \
	hnco/functions/controllers/controller.hh \
	hnco/functions/converter.hh \
	hnco/functions/decorator.hh \
	hnco/functions/function.hh \
	hnco/functions/modifiers/all.hh \
	hnco/functions/modifiers/modifier.hh \
	hnco/functions/modifiers/parsed-modifier.hh \
	hnco/functions/modifiers/prior-noise.hh \
	hnco/functions/multivariate-function-adapter.hh \
	hnco/functions/permutation-function-adapter.hh \
	hnco/functions/universal-function-adapter.hh \
	hnco/functions/universal-function.hh \
	hnco/functions/walsh-term.hh \
	hnco/iterator.hh \
	hnco/logging/log-context.hh \
	hnco/logging/logger.hh \
	hnco/maps/map.hh \
	hnco/maps/transvection.hh \
	hnco/neighborhoods/neighborhood-iterator.hh \
	hnco/neighborhoods/neighborhood.hh \
	hnco/permutation.hh \
	hnco/random.hh \
	hnco/representations/all.hh \
	hnco/representations/categorical.hh \
	hnco/representations/complex.hh \
	hnco/representations/float.hh \
	hnco/representations/integer.hh \
	hnco/representations/permutation.hh \
	hnco/serialization.hh \
	hnco/sparse-bit-vector.hh \
	hnco/stop-watch.hh \
	hnco/util.hh

if ENABLE_FACTORIZATION
nobase_include_HEADERS += hnco/functions/collection/factorization.hh
endif

if ENABLE_PLUGIN
nobase_include_HEADERS += hnco/functions/collection/plugin.hh
endif

BUILT_SOURCES = \
	hnco/app/hnco-options.cc \
	hnco/app/hnco-options.hh

hnco/app/hnco-options.hh hnco/app/hnco-options.cc: hnco/app/hnco.json
	cd $(top_srcdir)/lib/hnco/app && $(top_srcdir)/tools/optgen.pl hnco.json

EXTRA_DIST = \
	fast-efficient-p3/LICENSE.txt
