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

#include <assert.h>

#include <random>               // std::uniform_int_distribution

#include "hnco/exception.hh"

#include "steepest-ascent-hill-climbing.hh"


using namespace hnco::algorithm;
using namespace hnco::exception;
using namespace hnco::random;
using namespace hnco;


void
SteepestAscentHillClimbing::iterate()
{
  assert(_function);
  assert(_neighborhood);

  _neighborhood->init();
  assert(_neighborhood->has_next());

  // First element
  const bit_vector_t& bv = _neighborhood->next();
  double best_value = _function->evaluate(bv);
  size_t index = 0;
  _candidates[index++] = bv;

  // Other elements
  while (_neighborhood->has_next()) {
    const bit_vector_t& bv = _neighborhood->next(); // Hides previous bv
    double value = _function->evaluate(bv);
    if (value > best_value) {
      best_value = value;
      index = 0;
      _candidates[index++] = bv;
    } else if (value == best_value) {
      if (index < _candidates.size())
        _candidates[index++] = bv;
    }
  }

  assert(index >= 1);
  assert(index <= _candidates.size());

  if (best_value > _solution.second) {
    std::uniform_int_distribution<int> candidate_dist(0, index - 1);
    _solution.first = _candidates[candidate_dist(random::Generator::engine)];
    _solution.second = best_value;
    _neighborhood->set_origin(_solution.first);
  } else {
    _last_iteration = true;
  }
}
