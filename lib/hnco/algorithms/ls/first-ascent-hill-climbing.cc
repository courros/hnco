/* Copyright (C) 2016, 2017, 2018 Arnaud Berny

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

#include "hnco/exception.hh"

#include "first-ascent-hill-climbing.hh"


using namespace hnco::algorithm;
using namespace hnco::exception;
using namespace hnco;


void
FirstAscentHillClimbing::init()
{
  assert(_function);
  assert(_neighborhood);

  random_solution();
  _neighborhood->set_origin(_solution.first);
}


void
FirstAscentHillClimbing::init(const bit_vector_t& x)
{
  assert(_function);
  assert(_neighborhood);

  init(x, _function->eval(x));
}


void
FirstAscentHillClimbing::init(const bit_vector_t& x, double value)
{
  assert(_function);
  assert(_neighborhood);
  assert(value == _function->eval(x));

  set_solution(x, value);
  _neighborhood->set_origin(_solution.first);
}


void
FirstAscentHillClimbing::iterate()
{
  assert(_function);
  assert(_neighborhood);

  for (_neighborhood->init();
       _neighborhood->has_next();
       _neighborhood->next())
    {
      double value = _function->eval(_neighborhood->get_current());
      if (_operator(value, _solution.second)) {
        _solution.first = _neighborhood->get_current();
        _solution.second = value;
        _neighborhood->set_origin(_solution.first);
        return;
      }
    }

  throw LocalMaximum(_solution);
}
