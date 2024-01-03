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

#include "hnco/exception.hh"

#include "first-ascent-hill-climbing.hh"


using namespace hnco::algorithm;
using namespace hnco::exception;
using namespace hnco;


void
FirstAscentHillClimbing::iterate()
{
  assert(_function);
  assert(_neighborhood);

  _neighborhood->init();
  while (_neighborhood->has_next()) {
    const bit_vector_t& bv = _neighborhood->next();
    double value = _function->evaluate(bv);
    if (value > _solution.second) {
      _solution.first = bv;
      _solution.second = value;
      return;
    }
  }

  _last_iteration = true;
}
