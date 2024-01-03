/* Copyright (C) 2016, 2017, 2018, 2019, 2020, 2021, 2022, 2023, 2024 Arnaud Berny

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

#ifndef HNCO_MULTIOBJECTIVE_APP_APPLICATION_H
#define HNCO_MULTIOBJECTIVE_APP_APPLICATION_H

#include <vector>               // std::vector

#include "hnco-mo-options.hh"
#include "function-factory.hh"
#include "algorithm-factory.hh"


namespace hnco {
namespace multiobjective {
namespace app {


/// Command line application
class CommandLineApplication {

  /// HNCO options
  const HncoOptions& _options;

  /// Functin factory
  FunctionFactory& _function_factory;

  /// Algorithm factory
  AlgorithmFactory& _algorithm_factory;

  /// All functions
  std::vector<hnco::multiobjective::function::Function *> _fns;

  /// Main function
  hnco::multiobjective::function::Function *_fn = nullptr;

  /// Algorithm
  hnco::multiobjective::algorithm::Algorithm *_algorithm = nullptr;

  /// Log context
  logging::ProgressTrackerContext *_log_context = nullptr;

  /// Initialization
  void init();

  /// Make all functions
  void make_functions();

  /// Print information about the function
  void print_information();

  /// Make algorithm
  void make_algorithm();

  /// Minimize objective functions
  void minimize();

  /// Manage solutions
  void manage_solutions();

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
    , _function_factory(function_factory)
    , _algorithm_factory(algorithm_factory)
  {}

  /// Run the application
  void run();
};


} // end of namespace app
} // end of namespace multiobjective
} // end of namespace hnco


#endif
