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

#include <algorithm>            // std::is_sorted

#include "sparse-bit-vector.hh"


using namespace hnco;


bool
hnco::sbv_is_valid(const sparse_bit_vector_t& sbv)
{
  if (!std::is_sorted(begin(sbv), end(sbv)))
    return false;
  for (auto index : sbv)
    if(index < 0)
      return false;
  return true;
}

bool
hnco::sbv_is_valid(const sparse_bit_vector_t& sbv, int n)
{
  if (!sbv_is_valid(sbv))
    return false;
  for (auto index : sbv)
    if(!(index < n))
      return false;
  return true;
}

void
hnco::sbv_display(const sparse_bit_vector_t& v, std::ostream& stream)
{
  for (auto c : v)
    stream << c << " ";
}
