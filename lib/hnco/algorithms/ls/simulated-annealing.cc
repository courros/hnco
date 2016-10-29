/* Copyright (C) 2016 Arnaud Berny

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

#include <iostream>

#include "hnco/bit-vector.hh"
#include "hnco/functions/function.hh"
#include "hnco/random.hh"

#include "simulated-annealing.hh"


using namespace hnco::algorithm;
using namespace hnco::function;
using namespace hnco::neighborhood;
using namespace hnco::random;
using namespace std;


void
SimulatedAnnealing::set_beta()
{
  double delta = 0;
  int count = 0;
  for (int i = 0; i < _num_trials; i++) {
    bv_random(_solution);
    double a = _function->eval(_solution);
    _neighborhood->set_origin(_solution);
    _neighborhood->propose();
    double b = _function->eval(_neighborhood->get_candidate());
    if (b < a) {
      delta += b - a;
      count++;
    }
  }
  delta /= count;

  assert(_initial_acceptance_probability > 0);
  _beta = ::log(_initial_acceptance_probability) / delta;
}


void
SimulatedAnnealing::init()
{
  random_solution();
  _neighborhood->set_origin(_solution);
  _current = _maximum;
  _transitions = 0;
  set_beta();

}


void
SimulatedAnnealing::iterate()
{
  _neighborhood->propose();
  double value = _function->eval(_neighborhood->get_candidate());

  if (value >= _current) {
    _neighborhood->keep();
    _current = value;

    update_solution(_neighborhood->get_origin(), _current);

    // Not sure of that:
    // _transitions++;

  } else {
    if (Random::uniform() < exp((value - _current) * _beta)) {
      _neighborhood->keep();
      _current = value;

      _transitions++;
    }
    else
      _neighborhood->forget();
  }

  // Cooling schedule
  if (_transitions == _num_transitions) {
    _transitions = 0;
    _beta *= _rate;
  }

}
