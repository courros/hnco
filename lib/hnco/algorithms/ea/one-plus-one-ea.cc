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

#include <iostream>

#include "hnco/functions/function.hh"

#include "one-plus-one-ea.hh"


using namespace hnco::algorithm;
using namespace hnco::function;
using namespace hnco::neighborhood;
using namespace hnco;


void
OnePlusOneEa::init()
{
  random_solution();
  _neighborhood.set_origin(_solution.first);
}


void
OnePlusOneEa::iterate()
{
  _neighborhood.propose();
  double value = _function->eval(_neighborhood.get_candidate());

  if (value >= _solution.second) { // success
    _neighborhood.keep();
    set_solution(_neighborhood.get_origin(), value);
  } else                        // failure
    _neighborhood.forget();
}
