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

#include "trap.hh"


using namespace hnco::function;


double
Trap::evaluate(const bit_vector_t& x)
{
  double result = 0;
  for (int i = 0, first = 0, last = _trap_size;
       i < _num_traps;
       i++, first = last, last += _trap_size) {
    int weight = 0;
    assert(last - first == _trap_size);
    for (int j = first; j < last; j++)
      weight += x[j];
    if (weight == _trap_size)
      result += _trap_size;
    else {
      assert(weight < _trap_size);
      result += _trap_size - weight - 1;
    }
  }
  return result;
}
