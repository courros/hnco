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

#include "local-search.hh"


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
  assert(_function);
  assert(_neighborhood);

  _neighborhood->propose();
  double value = _function->eval(_neighborhood->get_candidate());

  if (value >= _solution.second) {     // success
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
SteepestAscentHillClimbing::init()
{
  assert(_function);
  assert(_neighborhood);

  random_solution();
  _neighborhood->set_origin(_solution.first);
}


void
SteepestAscentHillClimbing::init(const bit_vector_t& x)
{
  assert(_function);
  assert(_neighborhood);

  init(x, _function->eval(x));
}


void
SteepestAscentHillClimbing::init(const bit_vector_t& x, double value)
{
  assert(_function);
  assert(_neighborhood);
  assert(value == _function->eval(x));

  set_solution(x, value);
  _neighborhood->set_origin(_solution.first);
}


void
SteepestAscentHillClimbing::iterate()
{
  assert(_function);
  assert(_neighborhood);

  _neighborhood->init();
  double best_value = _function->eval(_neighborhood->get_current());
  size_t index = 0;
  _candidates[index++] = _neighborhood->get_current();

  while (_neighborhood->has_next()) {
    _neighborhood->next();
    double value = _function->eval(_neighborhood->get_current());
    if (value >= best_value) {
      if (value > best_value) {
        index = 0;
        best_value = value;
      }
      if (index < _candidates.size())
        _candidates[index++] = _neighborhood->get_current();
    }
  }

  assert(index >= 1);
  assert(index <= _candidates.size());

  if (best_value >= _solution.second) {
    std::uniform_int_distribution<int> choose_candidate(0, index - 1);
    _solution.first = _candidates[choose_candidate(random::Random::engine)];
    _solution.second = best_value;
    _neighborhood->set_origin(_solution.first);
  } else
    throw LocalMaximum(_solution);
}
