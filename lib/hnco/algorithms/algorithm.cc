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

#include <assert.h>

#include "hnco/functions/function.hh"

#include "algorithm.hh"


using namespace hnco::algorithm;
using namespace hnco::function;
using namespace hnco::random;


Algorithm::Algorithm(int n):
  _solution(n) {}


void
Algorithm::random_solution()
{
  assert(_function);

  bv_random(_solution);
  _maximum = _function->eval(_solution);
}

void
Algorithm::set_solution(const bit_vector_t& x)
{
  assert(_function);

  set_solution(x, _function->eval(x));
}

void
Algorithm::set_solution(const bit_vector_t& x, double value)
{
  assert(_function);
  assert(_function->eval(x) == value);

  _solution = x;
  _maximum = value;
}

void
Algorithm::update_solution(const bit_vector_t& x)
{
  assert(_function);

  update_solution(x, _function->eval(x));
}

void
Algorithm::update_solution(const bit_vector_t& x, double value)
{
  assert(_function);
  assert(_function->eval(x) == value);

  if (value > _maximum) {
    _solution = x;
    _maximum = value;
  }
}

void
IterativeAlgorithm::maximize()
{

  if (_num_iterations > 0) {
    for (_iteration = 0; _iteration < _num_iterations; _iteration++) {
      iterate();
      if (_something_to_log)
        log();
    }
  } else {
    for (_iteration = 0;; _iteration++) {
      iterate();
      if (_something_to_log)
        log();
    }

  }

}
