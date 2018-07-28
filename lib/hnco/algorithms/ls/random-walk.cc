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

#include "random-walk.hh"


using namespace hnco::algorithm;
using namespace hnco::exception;
using namespace hnco;


void
RandomWalk::init()
{
  assert(_function);
  assert(_neighborhood);

  random_solution();
  _neighborhood->set_origin(_solution.first);
}


void
RandomWalk::init(const bit_vector_t& x)
{
  assert(_function);
  assert(_neighborhood);

  init(x, _function->eval(x));
}


void
RandomWalk::init(const bit_vector_t& x, double value)
{
  assert(_function);
  assert(_neighborhood);

  set_solution(x, value);
  _neighborhood->set_origin(_solution.first);
}


void
RandomWalk::iterate()
{
  assert(_function);

  if (_incremental_evaluation &&
      _function->provides_incremental_evaluation())
    iterate_incremental();
  else
    iterate_full();
}


void
RandomWalk::iterate_full()
{
  assert(_function);
  assert(_neighborhood);

  _neighborhood->propose();
  _value = _function->eval(_neighborhood->get_candidate());
  _neighborhood->keep();
  update_solution(_neighborhood->get_candidate(), _value);
}


void
RandomWalk::iterate_incremental()
{
  assert(_function);
  assert(_neighborhood);

  _neighborhood->propose();
  _value = _function->incremental_eval(_neighborhood->get_origin(), _value,
                                       _neighborhood->get_flipped_bits());
  _neighborhood->keep();
  update_solution(_neighborhood->get_candidate(), _value);
}


void
RandomWalk::log()
{
  (*_stream) << _value << std::endl;
}
