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

#include <algorithm>            // std::max, std::min
#include <cassert>
#include <cmath>                // std::pow

#include "hnco/logging/logger.hh"

#include "self-adjusting-one-plus-one-ea.hh"

using namespace hnco::algorithm;
using namespace hnco;

void
SelfAdjustingOnePlusOneEa::init()
{
  _mutation_rate = _mutation_rate_init;
  _mutation.set_mutation_rate(_mutation_rate);
  _mutation.set_allow_no_mutation(_allow_no_mutation);
  _coefficient = std::pow(_update_strength, _success_ratio);

  random_solution();
  _mutation.set_origin(_solution.first);
  set_something_to_log();
}

void
SelfAdjustingOnePlusOneEa::iterate()
{
  assert(_function);

  if (_incremental_evaluation &&
      _function->provides_incremental_evaluation())
    iterate_incremental();
  else
    iterate_full();
}

void
SelfAdjustingOnePlusOneEa::iterate_full()
{
  assert(_function);

  _mutation.propose();
  double value = _function->evaluate(_mutation.get_candidate());

  if (value >= _solution.second) {
    // success
    _mutation.keep();
    _solution.second = value;
    _mutation_rate = std::min(_mutation_rate * _coefficient, _mutation_rate_max);
  } else {
    // failure
    _mutation.forget();
    _mutation_rate = std::max(_mutation_rate / _update_strength, _mutation_rate_min);
  }
  _mutation.set_mutation_rate(_mutation_rate);

}

void
SelfAdjustingOnePlusOneEa::iterate_incremental()
{
  assert(_function);

  _mutation.propose();
  double value =
    _function->evaluate_incrementally(_mutation.get_origin(),
                                      _solution.second,
                                      _mutation.get_flipped_bits());

  if (value >= _solution.second) {
    // success
    _mutation.keep();
    _solution.second = value;
    _mutation_rate = std::min(_mutation_rate * _coefficient, _mutation_rate_max);
  } else {
    // failure
    _mutation.forget();
    _mutation_rate = std::max(_mutation_rate / _update_strength, _mutation_rate_min);
  }
  _mutation.set_mutation_rate(_mutation_rate);

}

void
SelfAdjustingOnePlusOneEa::finalize()
{
  _solution.first = _mutation.get_origin();
  // _solution.second has been taken care of
}

void
SelfAdjustingOnePlusOneEa::log()
{
  assert(_something_to_log);
  logging::Logger logger(_log_context);
  if (_log_mutation_rate)
    logger << _mutation_rate;
}
