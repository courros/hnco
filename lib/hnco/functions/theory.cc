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

#include <algorithm>            // std::all_of, std::find
#include <bitset>

#include "hnco/exception.hh"

#include "theory.hh"


using namespace hnco::function;


double
OneMax::evaluate(const bit_vector_t& x)
{
  return bv_hamming_weight(x);
}

double
OneMax::evaluate_incrementally(const bit_vector_t& x, double v, const hnco::sparse_bit_vector_t& flipped_bits)
{
  double delta = 0;
  for (auto index : flipped_bits)
    if (x[index])
      delta--;
    else
      delta++;
  return v + delta;
}

double
LeadingOnes::evaluate(const bit_vector_t& x)
{
  size_t i = 0;
  for (;i < x.size(); i++)
    if (x[i] == 0)
      break;
  return i;
}

double
Needle::evaluate(const bit_vector_t& x)
{
  for (size_t i = 0; i < x.size(); i++)
    if (x[i] == 0)
      return 0;
  return 1;  
}

Hiff::Hiff(int bv_size):
  _bv_size(bv_size)
{
  assert(bv_size > 0);

  std::bitset<8 * sizeof(int)> bs(bv_size);
  if (bs.count() != 1)
    throw exception::Error("Hiff::Hiff: bv_size must be a power of 2");
  for (size_t i = 0; i < bs.size(); i++)
    if (bs.test(i)) {
      _depth = i;
      break;
    }
}

double
Hiff::evaluate(const bit_vector_t& x)
{
  double result = 0;
  for (int i = 0, num_blocks = _bv_size, block_size = 1; i <= _depth; i++, num_blocks /= 2, block_size *= 2) {
    bool last = true;
    for (int j = 0, offset = 0; j < num_blocks; j++, offset += block_size) {
      bit_t b0 = x[offset];
      if (all_of(x.begin() + offset, x.begin() + offset + block_size, [b0](bit_t b){ return b == b0; })) {
        result += block_size;
        last = false;
      }
    }
    if (last)
      break;
  }
  return result;  
}


double
Ridge::evaluate(const bit_vector_t& x)
{
  bit_vector_t::const_iterator it = find(x.begin(), x.end(), 0);

  if (all_of(it, x.end(), [](bit_t b){ return b == 0; })) {
    int length = it - x.begin();
    assert(bv_hamming_weight(x) == length);
    return _bv_size + length;
  }

  return _bv_size - bv_hamming_weight(x);
}


double
Plateau::evaluate(const bit_vector_t& x)
{
  bit_vector_t::const_iterator it = find(x.begin(), x.end(), 0);

  if (it == x.end()) {
    assert(bv_hamming_weight(x) == int(_bv_size));
    return _bv_size + 2;
  }

  if (all_of(it, x.end(), [](bit_t b){ return b == 0; }))
    return _bv_size + 1;

  return _bv_size - bv_hamming_weight(x);
}
