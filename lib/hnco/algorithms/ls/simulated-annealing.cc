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
#include <math.h>		// exp, log

#include "hnco/random.hh"

#include "simulated-annealing.hh"


using namespace hnco::algorithm;
using namespace hnco::random;


void
SimulatedAnnealing::init_beta()
{
  assert(_neighborhood);
  assert(_function);

  bit_vector_t bv(_solution.first);

  double delta = 0;
  int count = 0;
  for (int i = 0; i < _num_trials; i++) {
    bv_random(bv);
    double a = _function->evaluate(bv);
    update_solution(bv, a);
    _neighborhood->set_origin(bv);
    _neighborhood->propose();
    double b = _function->evaluate(_neighborhood->get_candidate());
    update_solution(_neighborhood->get_candidate(), b);
    if (b < a) {
      delta += b - a;
      count++;
    }
    _neighborhood->forget();
  }
  delta /= count;

  assert(_initial_acceptance_probability > 0);
  _beta = ::log(_initial_acceptance_probability) / delta;
}


void
SimulatedAnnealing::init()
{
  if (_random_initialization)
    random_solution();
  else
    set_solution(_starting_point);
  init_beta();
  _neighborhood->set_origin(_solution.first);
  _current_value = _solution.second;
  _transitions = 0;
}


void
SimulatedAnnealing::iterate()
{
  assert(_neighborhood);
  assert(_function);

  _neighborhood->propose();
  double value = _function->evaluate(_neighborhood->get_candidate());

  if (value >= _current_value) {
    _neighborhood->keep();
    _current_value = value;

    update_solution(_neighborhood->get_origin(), _current_value);

    // Not sure of that:
    // _transitions++;

  } else {
    if (Generator::uniform() < exp((value - _current_value) * _beta)) {
      _neighborhood->keep();
      _current_value = value;

      _transitions++;
    } else
      _neighborhood->forget();
  }

  // Cooling schedule
  if (_transitions == _num_transitions) {
    _transitions = 0;
    _beta *= _beta_ratio;
  }

}
