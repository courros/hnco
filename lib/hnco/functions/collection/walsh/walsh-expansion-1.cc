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

#include <iostream>

#include "hnco/random.hh"

#include "walsh-expansion-1.hh"


using namespace hnco::random;
using namespace hnco::function;


double
WalshExpansion1::get_maximum() const
{
  double result = 0;
  for (auto w : _linear)
    if (w > 0)
      result += w;
    else
      result -= w;
  return result;
}


double
WalshExpansion1::evaluate(const bit_vector_t& s)
{
  assert(s.size() == _linear.size());

  double result = 0;

  // Linear part
  for (size_t i = 0; i < _linear.size(); i++)
    if (s[i])
      result -= _linear[i];
    else
      result += _linear[i];

  return result;
}


double
WalshExpansion1::evaluate_incrementally(const bit_vector_t& x,
                                        double value,
                                        const hnco::sparse_bit_vector_t& flipped_bits)
{
  assert(x.size() == _linear.size());

  for (auto index : flipped_bits)
    if (x[index])
      value += 2 * _linear[index];
    else
      value -= 2 * _linear[index];
  return value;
}
