/* Copyright (C) 2016, 2017, 2018, 2019, 2020 Arnaud Berny

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

#include "local-search-algorithm.hh"


using namespace hnco::algorithm;
using namespace hnco;


void
LocalSearchAlgorithm::init()
{
  assert(_function);
  assert(_neighborhood);

  if (_random_initialization)
    random_solution();
  else
    set_solution(_starting_point);
  _neighborhood->set_origin(_solution.first);
}
