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

#include <algorithm>            // std::max, std::min, std::clamp
#include <cassert>

#include "hnco/logging/logger.hh"

#include "two-rate-one-plus-lambda-ea.hh"

using namespace hnco::algorithm;

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
  const int lambda = _population.get_size();
  const int mid = lambda / 2;
  const double low = _mutation_rate / 2;
  const double high = _mutation_rate * 2;

  // Generate population
  _mutation_operator.set_mutation_rate(low);
  for (int i = 0; i < mid; i++)
    _mutation_operator.map(_solution.first, _population.bvs[i]);

  _mutation_operator.set_mutation_rate(high);
  for (int i = mid; i < lambda; i++)
    _mutation_operator.map(_solution.first, _population.bvs[i]);

  // Evaluate and sort
  if (_functions.size() > 1)
    _population.evaluate_in_parallel(_functions);
  else
    _population.evaluate(_function);
  _population.sort();
  update_solution(_population.get_best_bv(), _population.get_best_value());

  // Update mutation rate
  double number = random::Generator::uniform();
  if (number < 0.25)
    _mutation_rate = std::max(low, _mutation_rate_min);
  else if (number < 0.5)
    _mutation_rate = std::min(high, _mutation_rate_max);
  else {
    if (_population.get_best_index(0) < mid)
      _mutation_rate = low;
    else
      _mutation_rate = high;
    _mutation_rate = std::clamp(_mutation_rate, _mutation_rate_min, _mutation_rate_max);
  }

}

void
TwoRateOnePlusLambdaEa::log()
{
  assert(_something_to_log);
  assert(_log_mutation_rate);
  logging::Logger logger(_log_context);
  logger << _mutation_rate;
}
