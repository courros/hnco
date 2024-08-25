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
	gomea/lib/cxxopts-3.1.1/include/cxxopts.hpp \
	gomea/src/common/gomea_defs.hpp \
	gomea/src/common/linkage_config.cpp \
	gomea/src/common/linkage_config.hpp \
	gomea/src/common/linkage_model.cpp \
	gomea/src/common/linkage_model.hpp \
	gomea/src/common/output_statistics.cpp \
	gomea/src/common/output_statistics.hpp \
	gomea/src/common/partial_solution.cpp \
	gomea/src/common/partial_solution.hpp \
	gomea/src/common/solution.cpp \
	gomea/src/common/solution.hpp \
	gomea/src/discrete/Config.cpp \
	gomea/src/discrete/Config.hpp \
	gomea/src/discrete/Population.cpp \
	gomea/src/discrete/Population.hpp \
	gomea/src/discrete/gomea.cpp \
	gomea/src/discrete/gomea.hpp \
	gomea/src/discrete/gomeaIMS.cpp \
	gomea/src/discrete/gomeaIMS.hpp \
	gomea/src/discrete/shared.hpp \
	gomea/src/discrete/utils.cpp \
	gomea/src/discrete/utils.hpp \
	gomea/src/fitness/bbo_fitness.cpp \
	gomea/src/fitness/bbo_fitness.hpp \
	gomea/src/fitness/benchmarks-discrete.hpp \
	gomea/src/fitness/fitness.cpp \
	gomea/src/fitness/fitness.hpp \
	gomea/src/fitness/gbo_fitness.hpp \
	gomea/src/utils/embed.hpp \
	gomea/src/utils/time.cpp \
	gomea/src/utils/time.hpp \
	gomea/src/utils/tools.cpp \
	gomea/src/utils/tools.hpp \
	hnco/algorithms/algorithm.cc \
	hnco/algorithms/algorithm.hh \
	hnco/algorithms/all.hh \
	hnco/algorithms/complete-search.cc \
	hnco/algorithms/complete-search.hh \
	hnco/algorithms/decorators/all.hh \
	hnco/algorithms/decorators/decorator.hh \
	hnco/algorithms/decorators/restart.cc \
	hnco/algorithms/decorators/restart.hh \
	hnco/algorithms/evolutionary-algorithms/all.hh \
	hnco/algorithms/evolutionary-algorithms/crossover.cc \
	hnco/algorithms/evolutionary-algorithms/crossover.hh \
	hnco/algorithms/evolutionary-algorithms/genetic-algorithm.cc \
	hnco/algorithms/evolutionary-algorithms/genetic-algorithm.hh \
	hnco/algorithms/evolutionary-algorithms/it-ea.cc \
	hnco/algorithms/evolutionary-algorithms/it-ea.hh \
	hnco/algorithms/evolutionary-algorithms/mu-comma-lambda-ea.cc \
	hnco/algorithms/evolutionary-algorithms/mu-comma-lambda-ea.hh \
	hnco/algorithms/evolutionary-algorithms/mu-plus-lambda-ea.cc \
	hnco/algorithms/evolutionary-algorithms/mu-plus-lambda-ea.hh \
	hnco/algorithms/evolutionary-algorithms/one-plus-lambda-comma-lambda-ga.cc \
	hnco/algorithms/evolutionary-algorithms/one-plus-lambda-comma-lambda-ga.hh \
	hnco/algorithms/evolutionary-algorithms/one-plus-one-ea.hh \
	hnco/algorithms/evolutionary-algorithms/random-selection.cc \
	hnco/algorithms/evolutionary-algorithms/random-selection.hh \
	hnco/algorithms/evolutionary-algorithms/selection.hh \
	hnco/algorithms/evolutionary-algorithms/self-adjusting-one-plus-one-ea.cc \
	hnco/algorithms/evolutionary-algorithms/self-adjusting-one-plus-one-ea.hh \
	hnco/algorithms/evolutionary-algorithms/two-rate-one-plus-lambda-ea.cc \
	hnco/algorithms/evolutionary-algorithms/two-rate-one-plus-lambda-ea.hh \
	hnco/algorithms/fast-efficient-p3/all.hh \
	hnco/algorithms/fast-efficient-p3/hboa.cc \
	hnco/algorithms/fast-efficient-p3/hboa.hh \
	hnco/algorithms/fast-efficient-p3/hnco-evaluator.hh \
	hnco/algorithms/fast-efficient-p3/implementation.hh \
	hnco/algorithms/fast-efficient-p3/ltga.cc \
	hnco/algorithms/fast-efficient-p3/ltga.hh \
	hnco/algorithms/fast-efficient-p3/p3.cc \
	hnco/algorithms/fast-efficient-p3/p3.hh \
	hnco/algorithms/gomea/all.hh \
	hnco/algorithms/gomea/gomea.cc \
	hnco/algorithms/gomea/gomea.hh \
	hnco/algorithms/gomea/hnco-fitness.hh \
	hnco/algorithms/human.cc \
	hnco/algorithms/human.hh \
	hnco/algorithms/iterative-algorithm.cc \
	hnco/algorithms/iterative-algorithm.hh \
	hnco/algorithms/local-search/all.hh \
	hnco/algorithms/local-search/first-ascent-hill-climbing.cc \
	hnco/algorithms/local-search/first-ascent-hill-climbing.hh \
	hnco/algorithms/local-search/local-search-algorithm.hh \
	hnco/algorithms/local-search/random-local-search.cc \
	hnco/algorithms/local-search/random-local-search.hh \
	hnco/algorithms/local-search/random-walk.cc \
	hnco/algorithms/local-search/random-walk.hh \
	hnco/algorithms/local-search/simulated-annealing.cc \
	hnco/algorithms/local-search/simulated-annealing.hh \
	hnco/algorithms/local-search/steepest-ascent-hill-climbing.cc \
	hnco/algorithms/local-search/steepest-ascent-hill-climbing.hh \
	hnco/algorithms/mimic.cc \
	hnco/algorithms/mimic.hh \
	hnco/algorithms/population.cc \
	hnco/algorithms/population.hh \
	hnco/algorithms/probability-vector/all.hh \
	hnco/algorithms/probability-vector/compact-ga.cc \
	hnco/algorithms/probability-vector/compact-ga.hh \
	hnco/algorithms/probability-vector/mmas.cc \
	hnco/algorithms/probability-vector/mmas.hh \
	hnco/algorithms/probability-vector/nps-pbil.cc \
	hnco/algorithms/probability-vector/nps-pbil.hh \
	hnco/algorithms/probability-vector/pbil.cc \
	hnco/algorithms/probability-vector/pbil.hh \
	hnco/algorithms/probability-vector/probability-vector.cc \
	hnco/algorithms/probability-vector/probability-vector.hh \
	hnco/algorithms/probability-vector/pv-algorithm.cc \
	hnco/algorithms/probability-vector/pv-algorithm.hh \
	hnco/algorithms/probability-vector/random-affine-map.cc \
	hnco/algorithms/probability-vector/random-affine-map.hh \
	hnco/algorithms/probability-vector/umda.cc \
	hnco/algorithms/probability-vector/umda.hh \
	hnco/algorithms/random-search.cc \
	hnco/algorithms/random-search.hh \
	hnco/algorithms/solution.hh \
	hnco/algorithms/walsh-moment/all.hh \
	hnco/algorithms/walsh-moment/bm-pbil.hh \
	hnco/algorithms/walsh-moment/gibbs-sampler.cc \
	hnco/algorithms/walsh-moment/gibbs-sampler.hh \
	hnco/algorithms/walsh-moment/hea.hh \
	hnco/algorithms/walsh-moment/herding.cc \
	hnco/algorithms/walsh-moment/herding.hh \
	hnco/algorithms/walsh-moment/matrix.hh \
	hnco/algorithms/walsh-moment/walsh-moment.cc \
	hnco/algorithms/walsh-moment/walsh-moment.hh \
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
	hnco/app/make-multivariate-function-adapter.hh \
	hnco/app/parser.cc \
	hnco/app/parser.hh \
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
	hnco/functions/multiobjective-function-adapter.hh \
	hnco/functions/multivariate-function-adapter.hh \
	hnco/functions/permutation-function-adapter.hh \
	hnco/functions/universal-function-adapter.hh \
	hnco/functions/universal-function.hh \
	hnco/functions/walsh-term.hh \
	hnco/iterator.cc \
	hnco/iterator.hh \
	hnco/logging/log-context.hh \
	hnco/logging/logger.hh \
	hnco/maps/map.cc \
	hnco/maps/map.hh \
	hnco/maps/transvection.cc \
	hnco/maps/transvection.hh \
	hnco/multiobjective/algorithms/algorithm.hh \
	hnco/multiobjective/algorithms/all.hh \
	hnco/multiobjective/algorithms/iterative-algorithm.cc \
	hnco/multiobjective/algorithms/iterative-algorithm.hh \
	hnco/multiobjective/algorithms/nsga2.cc \
	hnco/multiobjective/algorithms/nsga2.hh \
	hnco/multiobjective/algorithms/pareto-front-computation.hh \
	hnco/multiobjective/algorithms/population.cc \
	hnco/multiobjective/algorithms/population.hh \
	hnco/multiobjective/algorithms/random-selection.hh \
	hnco/multiobjective/app/algorithm-factory.cc \
	hnco/multiobjective/app/algorithm-factory.hh \
	hnco/multiobjective/app/application.cc \
	hnco/multiobjective/app/application.hh \
	hnco/multiobjective/app/function-factory.cc \
	hnco/multiobjective/app/function-factory.hh \
	hnco/multiobjective/app/hnco-mo-options.cc \
	hnco/multiobjective/app/hnco-mo-options.hh \
	hnco/multiobjective/app/hnco-mo.json \
	hnco/multiobjective/functions/all.hh \
	hnco/multiobjective/functions/collection/all.hh \
	hnco/multiobjective/functions/collection/parsed-multivariate-function.hh \
	hnco/multiobjective/functions/function.hh \
	hnco/multiobjective/functions/multivariate-function-adapter.hh \
	hnco/multiobjective/functions/universal-function-adapter.hh \
	hnco/multiobjective/functions/universal-function.hh \
	hnco/multiobjective/functions/value.hh \
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
	hnco/representations/value-set.hh \
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

if ENABLE_PYTHON
libhnco_la_SOURCES += \
	hnco/functions/collection/python-function.cc \
	hnco/functions/collection/python-function.hh \
	hnco/multiobjective/functions/collection/python-function.cc \
	hnco/multiobjective/functions/collection/python-function.hh
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
	gomea/lib/cxxopts-3.1.1/include/cxxopts.hpp \
	gomea/src/common/gomea_defs.hpp \
	gomea/src/common/linkage_config.hpp \
	gomea/src/common/linkage_model.hpp \
	gomea/src/common/output_statistics.hpp \
	gomea/src/common/partial_solution.hpp \
	gomea/src/common/solution.hpp \
	gomea/src/discrete/Config.hpp \
	gomea/src/discrete/Population.hpp \
	gomea/src/discrete/gomea.hpp \
	gomea/src/discrete/gomeaIMS.hpp \
	gomea/src/discrete/shared.hpp \
	gomea/src/discrete/utils.hpp \
	gomea/src/fitness/bbo_fitness.hpp \
	gomea/src/fitness/benchmarks-discrete.hpp \
	gomea/src/fitness/fitness.hpp \
	gomea/src/fitness/gbo_fitness.hpp \
	gomea/src/utils/embed.hpp \
	gomea/src/utils/time.hpp \
	gomea/src/utils/tools.hpp \
	hnco/algorithms/algorithm.hh \
	hnco/algorithms/all.hh \
	hnco/algorithms/complete-search.hh \
	hnco/algorithms/decorators/all.hh \
	hnco/algorithms/decorators/decorator.hh \
	hnco/algorithms/decorators/restart.hh \
	hnco/algorithms/evolutionary-algorithms/all.hh \
	hnco/algorithms/evolutionary-algorithms/crossover.hh \
	hnco/algorithms/evolutionary-algorithms/genetic-algorithm.hh \
	hnco/algorithms/evolutionary-algorithms/it-ea.hh \
	hnco/algorithms/evolutionary-algorithms/mu-comma-lambda-ea.hh \
	hnco/algorithms/evolutionary-algorithms/mu-plus-lambda-ea.hh \
	hnco/algorithms/evolutionary-algorithms/one-plus-lambda-comma-lambda-ga.hh \
	hnco/algorithms/evolutionary-algorithms/one-plus-one-ea.hh \
	hnco/algorithms/evolutionary-algorithms/random-selection.hh \
	hnco/algorithms/evolutionary-algorithms/selection.hh \
	hnco/algorithms/evolutionary-algorithms/self-adjusting-one-plus-one-ea.hh \
	hnco/algorithms/evolutionary-algorithms/two-rate-one-plus-lambda-ea.hh \
	hnco/algorithms/fast-efficient-p3/all.hh \
	hnco/algorithms/fast-efficient-p3/hboa.hh \
	hnco/algorithms/fast-efficient-p3/hnco-evaluator.hh \
	hnco/algorithms/fast-efficient-p3/implementation.hh \
	hnco/algorithms/fast-efficient-p3/ltga.hh \
	hnco/algorithms/fast-efficient-p3/p3.hh \
	hnco/algorithms/gomea/all.hh \
	hnco/algorithms/gomea/gomea.hh \
	hnco/algorithms/gomea/hnco-fitness.hh \
	hnco/algorithms/human.hh \
	hnco/algorithms/iterative-algorithm.hh \
	hnco/algorithms/local-search/all.hh \
	hnco/algorithms/local-search/first-ascent-hill-climbing.hh \
	hnco/algorithms/local-search/local-search-algorithm.hh \
	hnco/algorithms/local-search/random-local-search.hh \
	hnco/algorithms/local-search/random-walk.hh \
	hnco/algorithms/local-search/simulated-annealing.hh \
	hnco/algorithms/local-search/steepest-ascent-hill-climbing.hh \
	hnco/algorithms/mimic.hh \
	hnco/algorithms/population.hh \
	hnco/algorithms/probability-vector/all.hh \
	hnco/algorithms/probability-vector/compact-ga.hh \
	hnco/algorithms/probability-vector/mmas.hh \
	hnco/algorithms/probability-vector/nps-pbil.hh \
	hnco/algorithms/probability-vector/pbil.hh \
	hnco/algorithms/probability-vector/probability-vector.hh \
	hnco/algorithms/probability-vector/pv-algorithm.hh \
	hnco/algorithms/probability-vector/random-affine-map.hh \
	hnco/algorithms/probability-vector/umda.hh \
	hnco/algorithms/random-search.hh \
	hnco/algorithms/solution.hh \
	hnco/algorithms/walsh-moment/all.hh \
	hnco/algorithms/walsh-moment/bm-pbil.hh \
	hnco/algorithms/walsh-moment/gibbs-sampler.hh \
	hnco/algorithms/walsh-moment/hea.hh \
	hnco/algorithms/walsh-moment/herding.hh \
	hnco/algorithms/walsh-moment/matrix.hh \
	hnco/algorithms/walsh-moment/walsh-moment.hh \
	hnco/app/algorithm-factory.hh \
	hnco/app/application.hh \
	hnco/app/decorated-function-factory.hh \
	hnco/app/function-factory.hh \
	hnco/app/hnco-options.hh \
	hnco/app/make-multivariate-function-adapter.hh \
	hnco/app/parser.hh \
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
	hnco/functions/multiobjective-function-adapter.hh \
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
	hnco/multiobjective/algorithms/algorithm.hh \
	hnco/multiobjective/algorithms/all.hh \
	hnco/multiobjective/algorithms/iterative-algorithm.hh \
	hnco/multiobjective/algorithms/nsga2.hh \
	hnco/multiobjective/algorithms/pareto-front-computation.hh \
	hnco/multiobjective/algorithms/population.hh \
	hnco/multiobjective/algorithms/random-selection.hh \
	hnco/multiobjective/app/algorithm-factory.hh \
	hnco/multiobjective/app/application.hh \
	hnco/multiobjective/app/function-factory.hh \
	hnco/multiobjective/app/hnco-mo-options.hh \
	hnco/multiobjective/app/hnco-mo.json \
	hnco/multiobjective/functions/all.hh \
	hnco/multiobjective/functions/collection/all.hh \
	hnco/multiobjective/functions/collection/parsed-multivariate-function.hh \
	hnco/multiobjective/functions/function.hh \
	hnco/multiobjective/functions/multivariate-function-adapter.hh \
	hnco/multiobjective/functions/universal-function-adapter.hh \
	hnco/multiobjective/functions/universal-function.hh \
	hnco/multiobjective/functions/value.hh \
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
	hnco/representations/value-set.hh \
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

if ENABLE_PYTHON
nobase_include_HEADERS += \
	hnco/functions/collection/python-function.hh \
	hnco/multiobjective/functions/collection/python-function.hh
endif

BUILT_SOURCES = \
	hnco/app/hnco-options.cc \
	hnco/app/hnco-options.hh \
	hnco/multiobjective/app/hnco-mo-options.cc \
	hnco/multiobjective/app/hnco-mo-options.hh

hnco/app/hnco-options.hh hnco/app/hnco-options.cc: hnco/app/hnco.json
	cd $(top_srcdir)/lib/hnco/app && $(top_srcdir)/tools/optgen.pl hnco.json

hnco/multiobjective/app/hnco-mo-options.hh hnco/multiobjective/app/hnco-mo-options.cc: hnco/multiobjective/app/hnco-mo.json
	cd $(top_srcdir)/lib/hnco/multiobjective/app && $(top_srcdir)/tools/optgen.pl hnco-mo.json

EXTRA_DIST = \
	fast-efficient-p3/LICENSE.txt \
	gomea/LICENSE \
	gomea/lib/cxxopts-3.1.1/LICENSE
