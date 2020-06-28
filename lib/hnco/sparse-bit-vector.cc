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

#include "sparse-bit-vector.hh"


using namespace hnco;


void
hnco::bv_flip(bit_vector_t& x, const sparse_bit_vector_t& sbv)
{
  for (auto index : sbv) {
    assert(index < int(x.size()));
    assert(index >= 0);
    bv_flip(x, index);
  }
}

void
hnco::sbv_display(const sparse_bit_vector_t& v, std::ostream& stream)
{
  for (auto c : v)
    stream << c << " ";
}

sparse_bit_vector_t
hnco::sbv_from_bv(const bit_vector_t& bv)
{
  sparse_bit_vector_t result;
  result.reserve(bv_hamming_weight(bv));

  for (size_t i = 0; i < bv.size(); i++)
    if (bv[i])
      result.push_back(i);

  assert(int(result.size()) == bv_hamming_weight(bv));

  return result;
}
