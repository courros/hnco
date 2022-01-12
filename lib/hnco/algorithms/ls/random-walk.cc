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

#include "hnco/logging/logger.hh"

#include "random-walk.hh"


using namespace hnco::algorithm;
using namespace hnco;


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
  _value = _function->evaluate(_neighborhood->get_candidate());
  _neighborhood->keep();
  update_solution(_neighborhood->get_candidate(), _value);
}


void
RandomWalk::iterate_incremental()
{
  assert(_function);
  assert(_neighborhood);

  _neighborhood->propose();
  _value = _function->evaluate_incrementally(_neighborhood->get_origin(),
                                             _value,
                                             _neighborhood->get_flipped_bits());
  _neighborhood->keep();
  update_solution(_neighborhood->get_candidate(), _value);
}


void
RandomWalk::log()
{
  assert(_something_to_log);

  HNCO_LOG() << _value;
}
