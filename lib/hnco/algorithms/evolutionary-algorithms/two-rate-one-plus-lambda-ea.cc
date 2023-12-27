/* Copyright (C) 2016, 2017, 2018, 2019, 2020, 2021, 2022, 2023 Arnaud Berny

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

#include <algorithm>            // std::max, std::min, std::clamp

#include "hnco/logging/logger.hh"
#include "hnco/util.hh"         // hnco::is_in_range

#include "two-rate-one-plus-lambda-ea.hh"


using namespace hnco::algorithm;
using namespace hnco::function;
using namespace hnco::random;
using namespace hnco;


void
TwoRateOnePlusLambdaEa::init()
{
  random_solution();
  _mutation_rate = _mutation_rate_init;
  _mutation_operator.set_allow_no_mutation(_allow_no_mutation);
  set_something_to_log();
}


void
TwoRateOnePlusLambdaEa::iterate()
{
  // Generate population
  const int lambda = _population.get_size();
  const int mid = lambda / 2;

  _mutation_operator.set_mutation_rate(_mutation_rate / 2);
  for (int i = 0; i < mid; i++) {
    _mutation_operator.map(_solution.first, _population.bvs[i]);
    _created_with_small_rate[i] = 1;
  }

  _mutation_operator.set_mutation_rate(2 * _mutation_rate);
  for (int i = mid; i < lambda; i++) {
    _mutation_operator.map(_solution.first, _population.bvs[i]);
    _created_with_small_rate[i] = 0;
  }

  // Evaluate and sort
  if (_functions.size() > 1)
    _population.evaluate_in_parallel(_functions);
  else
    _population.evaluate(_function);
  _population.sort();

  if (_population.get_best_value() >= _solution.second) {
    _solution.first = _population.get_best_bv();
    _solution.second = _population.get_best_value();
  }

  // Update mutation rate
  const int n = get_bv_size();
  const double low = 2 / double(n);
  const double high = 0.25;
  double x = Generator::uniform();
  if (x < 0.25)
    _mutation_rate = std::max(_mutation_rate / 2, low);
  else if ( x < 0.5)
    _mutation_rate = std::min(2 * _mutation_rate, high);
  else {
    assert(is_in_range(_population.permutation[0], _created_with_small_rate.size()));
    if (_created_with_small_rate[_population.permutation[0]])
      _mutation_rate = _mutation_rate / 2;
    else
      _mutation_rate = 2 * _mutation_rate;
    _mutation_rate = std::clamp(_mutation_rate, low, high);
  }

}


void
TwoRateOnePlusLambdaEa::log()
{
  assert(_something_to_log);

  logging::Logger l(_log_context);

  if (_log_mutation_rate)
    l.line() << _mutation_rate << " ";

}
