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

#include <math.h>               // std::abs
#include <omp.h>                // omp_set_num_threads
#include <assert.h>

#include <algorithm>            // std::sort
#include <iostream>
#include <fstream>              // std::ifstream, std::ofstream

#include "hnco/algorithms/decorators/restart.hh"
#include "hnco/exception.hh"
#include "hnco/random.hh"
#include "hnco/stop-watch.hh"   // StopWatch

#include "application.hh"

using namespace hnco::algorithm;
using namespace hnco::app;
using namespace hnco::exception;
using namespace hnco::function::controller;
using namespace hnco::function::modifier;
using namespace hnco::function;
using namespace hnco::neighborhood;
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
    throw Error("CommandLineApplication::init: At least one thread is required");
  assert(num_threads >= 1);
  omp_set_num_threads(num_threads);

  _fns = std::vector<function::Function *>(num_threads);
}

void
CommandLineApplication::make_functions()
{
  for (size_t i = 0; i < _fns.size(); i++) {
    Generator::reset();
    _fns[i] = _decorated_function_factory.make_function_modifier();
  }
  _fns[0] = _decorated_function_factory.make_function_controller(_fns[0]);
  _fn = _fns[0];
}

void
CommandLineApplication::load_solution()
{
  if (_options.with_load_solution()) {
    std::ifstream stream(_options.get_solution_path());
    if (!stream.good()) {
      throw Error("CommandLineApplication::load_solution: Cannot open " + _options.get_solution_path());
    }
    bit_vector_t x = bv_from_stream(stream);
    if (int(x.size()) != _fn->get_bv_size())
      throw Error("CommandLineApplication::load_solution: Solution size and function bv_size must be equal");
    if (_options.with_print_results())
      std::cout << _fn->evaluate(x) << std::endl;
    if (_options.with_print_description())
      _fn->describe(x, std::cout);
    exit(0);
  }
}

void
CommandLineApplication::print_information()
{
  if (_options.with_fn_has_known_maximum()) {
    if (_fn->has_known_maximum())
      std::cout << "yes" << std::endl;
    else
      std::cout << "no" << std::endl;
    exit(0);
  }

  if (_options.with_fn_get_maximum()) {
    if (_fn->has_known_maximum()) {
      std::cout << _fn->get_maximum() << std::endl;
      exit(0);
    } else {
      throw Error("CommandLineApplication::print_information: Function with unknown maximum");
    }
  }

  if (_options.with_fn_get_bv_size()) {
    std::cout << _fn->get_bv_size() << std::endl;
    exit(0);
  }

  if (_options.with_fn_provides_incremental_evaluation()) {
    if (_fn->provides_incremental_evaluation())
      std::cout << "yes" << std::endl;
    else
      std::cout << "no" << std::endl;
    exit(0);
  }

  if (_options.with_fn_walsh_transform()) {
    if (_fn->get_bv_size() > 15)
      std::cerr << "Warning: Walsh transform computation might take a long time or fail due to large bv_size" << std::endl;
    std::vector<function::WalshTerm> terms;
    compute_walsh_transform(_fn, terms);
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
    exit(0);
  }

  if (_options.with_fn_display()) {
    _fn->display(std::cout);
    exit(0);
  }

  // Print information about the map
  if (_options.with_map_display()) {
    hnco::Map *map = _decorated_function_factory.get_map();
    if (map) {
      map->display(std::cout);
    }
    exit(0);
  }

}

void
CommandLineApplication::make_algorithm()
{
  _algorithm = _algorithm_factory.make(_fn->get_bv_size());

  // In hnco, this happened before set_log_context.
  if (_options.with_restart()) {
    _algorithm = new Restart(_options.get_bv_size(), _algorithm);
  }

  _log_context = new logging::ProgressTrackerContext(_decorated_function_factory.get_tracker());
  _algorithm->set_log_context(_log_context);

}

void
CommandLineApplication::print_results(double total_time, bool target_reached)
{
  std::ostringstream results;

  ProgressTracker *tracker = _decorated_function_factory.get_tracker();
  ProgressTracker::Event last_improvement = tracker->get_last_improvement();

  results << "{\n  \"value\": "                    << last_improvement.value;
  results << ",\n  \"num_evaluations\": "          << last_improvement.num_evaluations;
  results << ",\n  \"total_num_evaluations\": "    << tracker->get_num_calls();
  results << ",\n  \"total_time\": "               << total_time;
  results << ",\n  \"evaluation_time\": "          << tracker->get_evaluation_time();

  if (_options.with_stop_on_maximum() || _options.with_stop_on_target()) {
    if (target_reached)
      results << ",\n  \"success\": true";
    else
      results << ",\n  \"success\": false";
  }

  Cache *cache = _decorated_function_factory.get_cache();
  if (cache)
    results << ",\n  \"lookup_ratio\": " << cache->get_lookup_ratio();

  results << "\n}\n";

  // Print results
  if (_options.with_print_results())
    std::cout << results.str();

  // Save results
  if (_options.with_save_results()) {
    std::ofstream stream(_options.get_results_path());
    stream << results.str();
  }
}

void
CommandLineApplication::manage_solution(const bit_vector_t& bv)
{
  bit_vector_t concrete_solution = bv;

  if (_options.with_concrete_solution()) {
    hnco::Map *map = _decorated_function_factory.get_map();
    if (map) {
      concrete_solution.resize(map->get_output_size());
      map->map(bv, concrete_solution);
    }
  }

  // Print solution
  if (_options.with_print_solution()) {
    bv_display(concrete_solution, std::cout);
    std::cout << std::endl;
  }

  // Save solution
  if (_options.with_save_solution()) {
    std::ofstream stream(_options.get_solution_path());
    bv_display(concrete_solution, stream);
    stream << std::endl;
  }

  // Print description
  if (_options.with_print_description()) {
    _fn->describe(concrete_solution, std::cout);
  }

  // Save description
  if (_options.with_save_description()) {
    std::ofstream stream(_options.get_description_path());
    _fn->describe(concrete_solution, stream);
  }
}

void
CommandLineApplication::maximize()
{
  if (_options.with_print_header())
    std::cout << _options;

  solution_t solution;

  bool target_reached = false;

  StopWatch stop_watch;
  stop_watch.start();

  try {
    _algorithm->maximize(_fns);
    _algorithm->finalize();
    solution = _algorithm->get_solution();
  }
  catch (const TargetReached& e) {
    std::cerr << "Warning: CommandLineApplication::maximize: " << e.what() << std::endl;
    auto controller = _decorated_function_factory.get_stop_on_target();
    solution = controller->get_trigger();
    target_reached = true;
  }
  catch (const LastEvaluation& e) {
    std::cerr << "Warning: CommandLineApplication::maximize: " << e.what() << std::endl;
    _algorithm->finalize();
    solution = _algorithm->get_solution();
  }

  stop_watch.stop();

  print_results(stop_watch.get_total_time(), target_reached);
  manage_solution(solution.first);

  if (_options.with_stop_on_maximum() && !target_reached)
    exit(2);

  if (_options.with_stop_on_target() && !target_reached)
    exit(3);

  exit(0);
}

void
CommandLineApplication::run()
{
  init();
  make_functions();
  load_solution();
  print_information();
  make_algorithm();
  maximize();
}
