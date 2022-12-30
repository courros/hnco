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

#include "hnco/logging/logger.hh"

#include "two-rate-one-plus-lambda-ea.hh"


using namespace hnco::function;
using namespace hnco::algorithm;
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
  _mutation_operator.set_mutation_rate(_mutation_rate);
  for (auto& offspring : _population.bvs)
    _mutation_operator.map(_solution.first, offspring);

  // Evaluate and sort
  if (_functions.size() > 1)
    _population.evaluate_in_parallel(_functions);
  else
    _population.evaluate(_function);
  _population.sort();           // Complete sort is required by get_equivalent_bvs

  update_solution(_population.get_best_bv(), _population.get_best_value());

  // Update mutation rate

}


void
TwoRateOnePlusLambdaEa::log()
{
  assert(_something_to_log);

  logging::Logger l(_log_context);

  if (_log_mutation_rate)
    l.line() << _mutation_rate << " ";

}
