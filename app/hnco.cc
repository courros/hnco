/* Copyright (C) 2016, 2017, 2018, 2019, 2020 Arnaud Berny

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

#include <math.h>               // pow, abs
#include <omp.h>                // omp_set_num_threads

#include <algorithm>            // sort
#include <chrono>               // system_clock::now
#include <iostream>

#include "hnco/exception.hh"
#include "hnco/functions/all.hh"
#include "hnco/stop-watch.hh"   // StopWatch
#include "hnco/random.hh"

#include "hnco-options.hh"
#include "make-algorithm.hh"
#include "make-function.hh"


using namespace hnco::algorithm;
using namespace hnco::exception;
using namespace hnco::function;
using namespace hnco::function::controller;
using namespace hnco::random;
using namespace hnco;


int main(int argc, char *argv[])
{
  Options options(argc, argv);

  if (options.with_print_defaults()) {
    std::cout << options;
    return 0;
  }

  //
  // Seed random number generator
  //

  if (!options.set_seed()) {
    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    options.set_seed(seed);
  }
  Random::generator.seed(options.get_seed());

  //
  // OpenMP
  //

  int num_threads = options.get_num_threads();
  if (num_threads < 1) {
    std::cerr << "Error: At least one thread is required" << std::endl;
    return 1;
  }
  assert(num_threads >= 1);
  omp_set_num_threads(num_threads);

  //
  // Make fitness function
  //

  FunctionFactory function_factory;
  std::vector<function::Function *> fns(num_threads);
  for (int i = 0; i < num_threads; i++) {
    Random::generator.seed(options.get_seed());
    try {
      fns[i] = function_factory.make_function(options);
    }
    catch (const Error& e) {
      std::cerr << "Error: " << e.what() << std::endl;
      return 1;
    }
  }

  try {
    fns[0] = function_factory.make_function_controller(fns[0], options);
  }
  catch (const Error& e) {
    std::cerr << "Error: " << e.what() << std::endl;
    return 1;
  }

  Function *fn = fns[0];
  assert(options.get_bv_size() == int(fn->get_bv_size()));

  //
  // Load a solution
  //

  if (options.with_load_solution()) {
    std::ifstream stream(options.get_solution_path());
    if (!stream.good()) {
      std::cerr << "Cannot open " << options.get_solution_path() << std::endl;
      return 1;
    }
    bit_vector_t x;
    bv_from_stream(x, stream);
    if (int(x.size()) != fn->get_bv_size()) {
      std::cerr << "Error: Solution size and function bv_size must be equal" << std::endl;
      return 1;
    }
    if (options.with_print_results())
      std::cout << fn->eval(x) << std::endl;
    if (options.with_print_description())
      fn->describe(x, std::cout);
    return 0;
  }

  //
  // Print information about the function
  //

  if (options.with_fn_has_known_maximum()) {
    if (fn->has_known_maximum())
      std::cout << "yes" << std::endl;
    else
      std::cout << "no" << std::endl;
    return 0;
  }

  if (options.with_fn_get_maximum()) {
    if (fn->has_known_maximum()) {
      std::cout << fn->get_maximum() << std::endl;
      return 0;
    } else {
      std::cerr << "Error: Function with unknown maximum" << std::endl;
      return 1;
    }
  }

  if (options.with_fn_get_bv_size()) {
    std::cout << fn->get_bv_size() << std::endl;
    return 0;
  }

  if (options.with_fn_provides_incremental_evaluation()) {
    if (fn->provides_incremental_evaluation())
      std::cout << "yes" << std::endl;
    else
      std::cout << "no" << std::endl;
    return 0;
  }

  if (options.with_fn_walsh_transform()) {
    if (fn->get_bv_size() > 15)
      std::cerr << "Warning: Walsh transform computation might take a long time or fail due to large bv_size" << std::endl;
    std::vector<function::WalshTerm> terms;
    fn->compute_walsh_transform(terms);
    for (auto& t : terms)
      t.coefficient = std::abs(t.coefficient);
    std::sort(terms.begin(),
              terms.end(),
              [](const function::WalshTerm& a,
                 const function::WalshTerm& b){ return a.coefficient > b.coefficient; });
    assert(terms.size() > 0);
    double norm;
    if (bv_hamming_weight(terms[0].feature) == 0)
      norm = terms[1].coefficient;
    else
      norm = terms[0].coefficient;
    for (auto t : terms) {
      int weight = bv_hamming_weight(t.feature);
      double amplitude = t.coefficient / norm;
      if (amplitude > 1e-10 && weight > 0)
        std::cout << weight << " " << amplitude << std::endl;
    }
    return 0;
  }

  if (options.with_fn_display()) {
    fn->display(std::cout);
    return 0;
  }

  //
  // Print information about the map
  //

  if (options.with_map_display()) {
    hnco::Map *map = function_factory.get_map();
    if (map) {
      map->display(std::cerr);
    }
  }

  //
  // Progress tracker
  //

  ProgressTracker *tracker = function_factory.get_tracker();

  //
  // Algorithm
  //

  Algorithm *algorithm;

  try { algorithm = make_algorithm(options); }
  catch (const Error& e) {
    std::cerr << "Error: " << e.what() << std::endl;
    return 1;
  }

  // Connect algorithm to functions
  algorithm->set_function(fn);
  algorithm->set_functions(fns);

  // Connect algorithm to log context
  ProgressTrackerContext log_context(tracker);
  algorithm->set_log_context(&log_context);

  //
  // Print header
  //

  if (options.with_print_header())
    std::cout << options;

  //
  // Maximize
  //

  point_value_t solution;

  bool maximum_reached = false;
  bool target_reached = false;
  bool initialized = false;

  StopWatch stop_watch;
  stop_watch.start();

  try {
    algorithm->init();
    initialized = true;
    algorithm->maximize();
    solution = algorithm->get_solution();
  }
  catch (const LocalMaximum& e) {
    solution = e.get_point_value();
  }
  catch (const MaximumReached& e) {
    solution = e.get_point_value();
    maximum_reached = true;
  }
  catch (const TargetReached& e) {
    solution = e.get_point_value();
    target_reached = true;
  }
  catch (LastEvaluation) {
    solution = algorithm->get_solution();
  }
  catch (const Error& e) {
    std::cerr << "Error: " << e.what() << std::endl;
    return 1;
  }

  stop_watch.stop();
  double total_time = stop_watch.get_total_time();

  if (!initialized)
    std::cerr << "Warning: Initialization has not been completed" << std::endl;

  //
  // Results
  //

  std::ostringstream results;

  results << "{\n";

  ProgressTracker::Event last_improvement = tracker->get_last_improvement();

  results << "  \"value\": "                    << last_improvement.value << ",\n";
  results << "  \"num_evaluations\": "          << last_improvement.num_evaluations << ",\n";
  results << "  \"total_num_evaluations\": "    << tracker->get_num_calls() << ",\n";
  results << "  \"total_time\": "               << total_time << ",\n";
  results << "  \"evaluation_time\": "          << tracker->get_evaluation_time();

  hnco::function::controller::Cache *cache = function_factory.get_cache();

  if (cache)
    results << ",\n  \"lookup_ratio\": " << cache->get_lookup_ratio();

  results << "\n}\n";

  // Print results
  if (options.with_print_results())
    std::cout << results.str();

  // Save results
  if (options.with_save_results()) {
    std::ofstream stream(options.get_results_path());
    stream << results.str();
  }

  //
  // Solution
  //

  bit_vector_t concrete_solution = solution.first;

  if (options.with_concrete_solution()) {
    hnco::Map *map = function_factory.get_map();
    if (map) {
      concrete_solution.resize(map->get_output_size());
      map->map(solution.first, concrete_solution);
    }
  }

  // Print solution
  if (options.with_print_solution()) {
    bv_display(concrete_solution, std::cout);
    std::cout << std::endl;
  }

  // Save solution
  if (options.with_save_solution()) {
    std::ofstream stream(options.get_solution_path());
    bv_display(concrete_solution, stream);
    stream << std::endl;
  }

  //
  // Description
  //

  // Print description
  if (options.with_print_description()) {
    fn->describe(concrete_solution, std::cout);
  }

  // Save description
  if (options.with_save_description()) {
    std::ofstream stream(options.get_description_path());
    fn->describe(concrete_solution, stream);
  }

  //
  // Exit status
  //

  if (options.with_stop_on_maximum() && !maximum_reached)
    return 2;

  if (options.with_stop_on_target() && !target_reached)
    return 3;

  return 0;
}
