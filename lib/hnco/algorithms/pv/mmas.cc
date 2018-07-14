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

#include "mmas.hh"

using namespace hnco::algorithm;


void
Mmas::init()
{
  random_solution();
  pv_uniform(_pv);

  // Update probability vector
  pv_update(_pv, _rate, _solution.first);
  pv_bound(_pv, _lower_bound, _upper_bound);
}

void
Mmas::iterate()
{
  pv_sample(_pv, _x);
  double value = _function->eval(_x);
  if (_compare(value, _solution.second)) {
    _solution.first = _x;
    _solution.second = value;
  }

  // Update probability vector
  pv_update(_pv, _rate, _solution.first);
  pv_bound(_pv, _lower_bound, _upper_bound);
}
