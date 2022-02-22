/* Copyright (C) 2016, 2017, 2018, 2019, 2020, 2021, 2022 Arnaud Berny

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

#include <assert.h>
#include <math.h>               // std::abs
#include <omp.h>                // omp_set_num_threads

#include <algorithm>            // std::sort
#include <fstream>              // std::ifstream, std::ofstream
#include <iostream>

#include "hnco/multiobjective/algorithms/pareto-front-computation.hh"
#include "hnco/random.hh"
#include "hnco/stop-watch.hh"   // StopWatch

#include "application.hh"


using namespace hnco::multiobjective::algorithm;
using namespace hnco::multiobjective::app;
using namespace hnco::multiobjective::function;
using namespace hnco::random;
using namespace hnco;


void
CommandLineApplication::init()
{
  if (_options.with_print_defaults()) {
    std::cout << _options;
    exit(0);
  }

  // Seed random number generator
  if (_options.set_seed())
    Generator::set_seed(_options.get_seed());
  else
    Generator::set_seed();

  // OpenMP
  int num_threads = _options.get_num_threads();
  if (num_threads < 1)
    throw std::runtime_error("CommandLineApplication::init: At least one thread is required");
  assert(num_threads >= 1);
  omp_set_num_threads(num_threads);

  _fns = std::vector<hnco::multiobjective::function::Function *>(num_threads);
}

void
CommandLineApplication::make_functions()
{
  for (size_t i = 0; i < _fns.size(); i++) {
    Generator::reset();
    _fns[i] = _function_factory.make();
  }
  _fn = _fns[0];

  if (_fn->get_bv_size() != _options.get_bv_size()) {
    std::cerr << "Warning: CommandLineApplication::make_function: After _function_factory.make(), bv_size changed from "
              << _options.get_bv_size() << " to "
              << _fn->get_bv_size() << std::endl;
  }

}

void
CommandLineApplication::print_information()
{
  if (_options.with_fn_get_bv_size()) {
    std::cout << _fn->get_bv_size() << std::endl;
    exit(0);
  }

  if (_options.with_fn_display()) {
    _fn->display(std::cout);
    exit(0);
  }

}

void
CommandLineApplication::make_algorithm()
{
  _algorithm = _algorithm_factory.make(_fn->get_bv_size(), _fn->get_output_size());

}

void
CommandLineApplication::minimize()
{
  if (_options.with_print_header())
    std::cout << _options;

  StopWatch stop_watch;
  stop_watch.start();

  _algorithm->minimize(_fns);
  _algorithm->finalize();

  stop_watch.stop();

}

void
CommandLineApplication::manage_solutions()
{
  Population solutions = _algorithm->get_solutions();
  Nsga2ParetoFrontComputation pareto_front_computation(solutions);
  std::vector<int> pareto_fronts(solutions.size());
  pareto_front_computation.compute(pareto_fronts);

  for (int i = 0; i < solutions.size(); i++) {
    if (pareto_fronts[i] == 0) {
      for (auto x : solutions.values[i])
        std::cout << x << " ";
      std::cout << std::endl;
    }
  }
}

void
CommandLineApplication::run()
{
  init();
  make_functions();
  print_information();
  make_algorithm();
  minimize();
  manage_solutions();
  exit(0);
}
