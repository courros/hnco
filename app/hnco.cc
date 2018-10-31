/* Copyright (C) 2016, 2017, 2018 Arnaud Berny

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
#include <ctime>                // clock
#include <iostream>

#include "hnco/exception.hh"
#include "hnco/functions/all.hh"
#include "hnco/random.hh"

#include "hnco-options.hh"
#include "make-algorithm.hh"
#include "make-function.hh"


using namespace hnco::algorithm;
using namespace hnco::exception;
using namespace hnco::function;
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
  Random::engine.seed(options.get_seed());

  //
  // Make fitness function
  //

  FunctionFactory function_factory;

  Function *fn;
  try { fn = function_factory.make_function(options); }
  catch (const Error& e) {
    std::cerr << "Error: " << e.what() << std::endl;
    return 1;
  }
  assert(options.get_bv_size() == int(fn->get_bv_size()));

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
      std::cerr << "Error: function with unknown maximum" << std::endl;
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
    std::vector<Function::WalshTransformTerm> terms;
    fn->compute_walsh_transform(terms);
    for (auto& t : terms)
      t.coefficient = std::abs(t.coefficient);
    std::sort(terms.begin(),
              terms.end(),
              [](const Function::WalshTransformTerm& a,
                 const Function::WalshTransformTerm& b){ return a.coefficient > b.coefficient; });
    double norm = terms[0].coefficient;
    for (auto t : terms) {
      double relative = t.coefficient / norm;
      if (relative > 1e-10)
        std::cout << bv_hamming_weight(t.feature) << " " << relative << std::endl;
    }
    return 0;
  }

  if (options.with_fn_display()) {
    fn->display(std::cout);
    return 0;
  }

  //
  // Progress tracker
  //

  ProgressTracker *tracker = new ProgressTracker(fn);

  tracker->set_log_improvement(options.with_log_improvement());

  //
  // OpenMP
  //

  int num_threads = options.get_num_threads();
  if (num_threads < 1) {
    std::cerr << "Error: at least one thread is required" << std::endl;
    return 1;
  }
  assert(num_threads >= 1);
  omp_set_num_threads(num_threads);

  // Functions
  std::vector<function::Function *> fns(num_threads);
  fns[0] = tracker;
  for (int i = 1; i < num_threads; i++) {
    Random::engine.seed(options.get_seed());
    try { fns[i] = function_factory.make_function(options); }
    catch (const Error& e) {
      std::cerr << "Error: " << e.what() << std::endl;
      return 1;
    }
  }

  //
  // Algorithm
  //

  Algorithm *algorithm;

  try { algorithm = make_algorithm(options); }
  catch (const Error& e) {
    std::cerr << "Error: " << e.what() << std::endl;
    return 1;
  }

  // Connect algorithm and function
  algorithm->set_function(tracker);
  algorithm->set_functions(fns);

  //
  // Print header
  //

  if (options.with_print_header())
    std::cout << options;

  //
  // Initialization
  //

  try { algorithm->init(); }
  catch (LastEvaluation) {
    std::cerr << "Error: Not enough evaluations for initialization" << std::endl;
    std::cout << tracker->get_last_improvement() << std::endl;
    return 1;
  }
  catch (MaximumReached) {
    std::cerr << "Warning: Maximum reached during initialization" << std::endl;
    std::cout << tracker->get_last_improvement() << std::endl;
    return 1;
  }
  catch (TargetReached) {
    std::cerr << "Warning: Target reached during initialization" << std::endl;
    std::cout << tracker->get_last_improvement() << std::endl;
    return 1;
  }
  catch (const Error& e) {
    std::cerr << "Error: " << e.what() << std::endl;
    return 1;
  }

  //
  // Maximize
  //

  point_value_t solution;

  bool maximum_reached = false;
  bool target_reached = false;

  clock_t start_point = clock();

  try {
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

  double total_time = double(clock() - start_point) / CLOCKS_PER_SEC;

  //
  // Results
  //

  std::ostringstream results;
  results << "{\n";

  ProgressTracker::Event last_improvement = tracker->get_last_improvement();
  results << "  \"value\": " << last_improvement.value << ",\n";
  results << "  \"num_evaluations\": " << last_improvement.num_evaluations << ",\n";
  results << "  \"total_time\": " << total_time << ",\n";
  results << "  \"evaluation_time\": " << tracker->get_evaluation_time();
  hnco::function::Cache *cache = function_factory.get_cache();
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

  // Print solution
  if (options.with_print_solution()) {
    bv_display(solution.first, std::cout);
    std::cout << std::endl;
  }

  // Print concrete solution
  if (options.with_print_concrete_solution()) {
    hnco::Map *map = function_factory.get_map();
    if (map) {
      bit_vector_t concrete_solution(map->get_output_size());
      map->map(solution.first, concrete_solution);
      bv_display(concrete_solution, std::cout);
    } else {
      bv_display(solution.first, std::cout);
    }
    std::cout << std::endl;
  }

  // Describe solution
  if (options.with_describe_solution()) {
    tracker->describe(solution.first, std::cout);
  }

  if (options.with_stop_on_maximum() && !maximum_reached)
    return 2;

  if (options.with_stop_on_target() && !target_reached)
    return 3;

  return 0;
}
