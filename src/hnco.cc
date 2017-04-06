/* Copyright (C) 2016, 2017 Arnaud Berny

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

#include <iostream>
#include <chrono>

#include "hnco/exception.hh"
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

  if (options.with_print_default_parameters()) {
    std::cout << options;
    return 0;
  }

  if (!options.set_seed()) {
    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    options.set_seed(seed);
  }
  Random::engine.seed(options.get_seed());

  // ProgressTracker
  ProgressTracker *function;

  int num_threads = options.get_num_threads();
  if (num_threads < 1) {
    std::cerr << "Error: at least one thread is required" << std::endl;
    return 1;
  }
  assert(num_threads >= 1);

  std::vector<function::Function *> functions(num_threads);

  try { function = make_function(options); }
  catch (Error& e) {
    std::cerr << "Error: " << e.what() << std::endl;
    exit(1);
  }

  // Algorithm
  Algorithm *algorithm;

  try { algorithm = make_algorithm(options); }
  catch (Error& e) {
    std::cerr << "Error: " << e.what() << std::endl;
    exit(1);
  }

  // Connect algorithm and function
  algorithm->set_function(function);

  // Header
  if (!options.with_no_header())
    std::cout << options;

  // Initialization
  try { algorithm->init(); }
  catch (LastEvaluation) {
    std::cerr << "Error: Not enough evaluations for initialization" << std::endl;
    std::cout << function->get_last_improvement() << std::endl;
    exit(1);
  }
  catch (MaximumReached) {
    std::cerr << "Warning: Maximum reached during initialization" << std::endl;
    std::cout << function->get_last_improvement() << std::endl;
    exit(1);
  }
  catch (Error& e) {
    std::cerr << "Error: " << e.what() << std::endl;
    exit(1);
  }

  // Maximization
  try { algorithm->maximize(); }
  catch (LocalMaximum) {}
  catch (LastEvaluation) {}
  catch (MaximumReached) {}
  catch (Error& e) {
    std::cerr << "Error: " << e.what() << std::endl;
    exit(1);
  }

  // Maybe not up to date in case of MaximumReached

  // Print performances
  if (options.with_print_performances())
    std::cout << function->get_last_improvement() << std::endl;

  // Print solution
  if (options.with_print_solution()) {
    bv_display(algorithm->get_solution(), std::cout);
    std::cout << std::endl;
  }

  // Describe solution
  if (options.with_describe_solution()) {
    function->describe(algorithm->get_solution(), std::cout);
  }

  return 0;
}
