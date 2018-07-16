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

#include "bit-vector.hh"

using namespace hnco;


void check_libhnco(void) {}


std::size_t
hnco::bv_to_size_type(const bit_vector_t& x)
{
  assert(x.size() <= 8 * sizeof(std::size_t));

  std::size_t result = 0;
  std::size_t power = 1;
  for (size_t i = 0; i < x.size(); i++) {
    if (x[i])
      result |= power;
    power <<= 1;
  }
  return result;
}

void
hnco::bv_from_size_type(bit_vector_t& x, std::size_t index)
{
  assert(x.size() <= 8 * sizeof(std::size_t));

  std::size_t mask = 1;
  for (size_t i = 0; i < x.size(); i++) {
    if (index & mask)
      x[i] = 1;
    else
      x[i] = 0;
    mask <<= 1;
  }
}
