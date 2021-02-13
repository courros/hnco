/* Copyright (C) 2016, 2017, 2018, 2019, 2020, 2021 Arnaud Berny

   This file is part of HNCO.

   HNCO is free software: you can redistribute it and/or modify it
   under the terms of the GNU Lesser General Public License as
   published by the Free Software Foundation, either version 3 of the
   License, or (at your option) any later version.

   HNCO is distributed in the hope that it will be useful, but WITHOUT
   ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
   or FITNESS FOR A PARTICULAR PURPOSE. See the GNU Lesser General
   Public License for more details.

   You should have received a copy of the GNU Lesser General Public
   License along with HNCO. If not, see
   <http://www.gnu.org/licenses/>.

*/

#ifndef HNCO_APP_APPLICATION_H
#define HNCO_APP_APPLICATION_H

#include <vector>               // std::vector

#include "hnco-options.hh"
#include "decorated-function-factory.hh"
#include "algorithm-factory.hh"

namespace hnco {
namespace app {

/// Command line application
class CommandLineApplication {

  /// HNCO options
  const HncoOptions& _options;

  /// Decorated functin factory
  DecoratedFunctionFactory _decorated_function_factory;

  /// Algorithm factory
  AlgorithmFactory& _algorithm_factory;

  /// All functions
  std::vector<function::Function *> _fns;

  /// Main function
  function::Function *_fn = nullptr;

  /// Algorithm
  hnco::algorithm::Algorithm *_algorithm = nullptr;

  /// Log context
  logging::ProgressTrackerContext *_log_context = nullptr;

  /// Initialization
  void init();

  /// Make all functions
  void make_functions();

  /// Load a solution
  void load_solution();

  /// Print information about the function
  void print_information();

  /// Make algorithm
  void make_algorithm();

  /// Maximize the function
  void maximize();

  /// Print results
  void print_results(double total_time, bool maximum_reached, bool target_reached);

  /// Manage solution
  void manage_solution(const bit_vector_t& bv);

public:

  /** Constructor.

      \param options HNCO options
      \param function_factory Function factory
      \param algorithm_factory Algorithm factory
  */
  CommandLineApplication(const HncoOptions& options,
                         FunctionFactory& function_factory,
                         AlgorithmFactory& algorithm_factory)
    : _options(options)
    , _decorated_function_factory(options, function_factory)
    , _algorithm_factory(algorithm_factory) {};

  /// Run the application
  void run();
};

}
}

#endif
