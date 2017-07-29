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

#include <random>               // uniform_int_distribution

#include "random-local-search.hh"


using namespace hnco::algorithm;
using namespace hnco::exception;
using namespace hnco::random;
using namespace hnco;


void
RandomLocalSearch::init()
{
  assert(_function);
  assert(_neighborhood);

  random_solution();
  _neighborhood->set_origin(_solution.first);
  _failures = 0;
}


void
RandomLocalSearch::init(const bit_vector_t& x)
{
  assert(_function);
  assert(_neighborhood);

  init(x, _function->eval(x));
}


void
RandomLocalSearch::init(const bit_vector_t& x, double value)
{
  assert(_function);
  assert(value == _function->eval(x));
  assert(_neighborhood);

  set_solution(x, value);
  _neighborhood->set_origin(_solution.first);
  _failures = 0;
}


const point_value_t&
RandomLocalSearch::get_solution()
{
  assert(_neighborhood);
  _solution.first =  _neighborhood->get_origin();
  return _solution;
}


void
StrictRandomLocalSearch::iterate()
{
  assert(_function);
  assert(_neighborhood);

  _neighborhood->propose();
  double value = _function->eval(_neighborhood->get_candidate());

  if (value > _solution.second) { // success
    _neighborhood->keep();
    _solution.second = value;
    _failures = 0;
  }
  else {                        // failure
    _neighborhood->forget();
    _failures++;
  }

  if (_patience > 0 &&
      _failures == _patience)
    throw LocalMaximum(_solution);

}


void
NonStrictRandomLocalSearch::iterate()
{
  if (_incremental_evaluation)
    iterate_incremental();
  else
    iterate_full();
}


void
NonStrictRandomLocalSearch::iterate_full()
{
  assert(_function);
  assert(_neighborhood);

  _neighborhood->propose();
  double value = _function->eval(_neighborhood->get_candidate());

  if (value >= _solution.second) {
    // success
    _neighborhood->keep();
    _solution.second = value;
    _failures = 0;
  }
  else {
    // failure
    _neighborhood->forget();
    _failures++;
  }

  if (_patience > 0 &&
      _failures == _patience)
    throw LocalMaximum(_solution);

}


void
NonStrictRandomLocalSearch::iterate_incremental()
{
  assert(_function);
  assert(_neighborhood);

  _neighborhood->propose();
  double delta = _function->delta(_neighborhood->get_origin(), _solution.second, _neighborhood->get_flipped_bits());

  if (delta >= 0) {
    // success
    _neighborhood->keep();
    _solution.second += delta;
    _failures = 0;
  }
  else {
    // failure
    _neighborhood->forget();
    _failures++;
  }

  if (_patience > 0 &&
      _failures == _patience)
    throw LocalMaximum(_solution);

}
