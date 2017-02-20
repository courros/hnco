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

#include <assert.h>
#include <math.h>

#include <algorithm>            // std::all_of, std::find

#include "hnco/exception.hh"

#include "long-path.hh"


using namespace hnco::exception;
using namespace hnco::function;
using namespace hnco;


LongPath::LongPath(int bv_size, int prefix_length):
  _bv_size(bv_size),
  _prefix_length(prefix_length)
{
  assert(bv_size > 0);
  assert(prefix_length > 0);

  if (bv_size % prefix_length != 0)
    throw exception::Error("LongPath::LongPath: _bv_size must be a multiple of _prefix_length");

  if (_prefix_length < 2)
    throw Error("LongPath::LongPath: _prefix_length must be > 1");
}


// Match 0+1+
bool is_bridge(bit_vector_t::const_iterator first, bit_vector_t::const_iterator last)
{
  if (*first != 0)
    return false;

  bit_vector_t::const_iterator iter = std::find(first, last, 1);

  if (iter == last)
    return false;

  return std::all_of(iter, last, [](bit_t b){ return b == 1; });
}


double compute_index(bit_vector_t::const_iterator first, bit_vector_t::const_iterator last, int k)
{
  if (first == last)
    return 0;

  bit_vector_t::const_iterator prefix_last = first + k;

  // Front
  if (std::all_of(first, prefix_last, [](bit_t b){ return b == 0; }))
    return compute_index(prefix_last, last, k);

  // Bridge
  if (is_bridge(first, prefix_last)) {
    // Length of the long path of dimension n - k
    double path_length = k * std::pow(2, (last - prefix_last) / k) - (k - 1);
    if (compute_index(prefix_last, last, k) == path_length - 1) {
      // Number of heading zeros
      int i = std::find(first, prefix_last, 1) - first;
      // Hence (k - i) is equal to the index of the suffix in the bridge
      return path_length + (k - i) - 1;
    } else
      return -1;
  }

  // Back
  if (std::all_of(first, prefix_last, [](bit_t b){ return b == 1; })) {
    double index = compute_index(prefix_last, last, k);
    if (index < 0)
      return -1;
    else {
      // Length of the long path of dimension n - k
      double path_length = k * std::pow(2, (last - prefix_last) / k) - (k - 1);
      return path_length + (k - 1) + (path_length - 1) - index;
    }
  }

  return -1;
}


double
LongPath::eval(const bit_vector_t& x)
{
  assert(x.size() == _bv_size);

  double result = _bv_size * _bv_size;
  double index = compute_index(x.begin(), x.end(), _prefix_length);

  if (index < 0) {
    result -=
      _bv_size *
      std::accumulate(x.begin(), x.begin() + _prefix_length, 0.0) +
      std::accumulate(x.begin() + _prefix_length, x.end(), 0.0);
  } else {
    result += index + 1;
  }

  return result;
}
