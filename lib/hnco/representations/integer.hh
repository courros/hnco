/* Copyright (C) 2016, 2017, 2018, 2019, 2020, 2021, 2022 Arnaud Berny

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

#ifndef HNCO_REPRESENTATIONS_INTEGER_H
#define HNCO_REPRESENTATIONS_INTEGER_H

#include <assert.h>

#include <vector>
#include <iostream>             // std::ostream
#include <cmath>                // std::log, std::ceil
#include <complex>              // std::complex
#include <bitset>               // std::bitset

#include "hnco/util.hh"         // hnco::is_in_range
#include "hnco/bit-vector.hh"
#include "hnco/bit-matrix.hh"
#include "hnco/iterator.hh"


namespace hnco {
/// Representations
namespace representation {


/** Check whether the difference is safe.

    The template parameter T must be an integral type such as int or
    long.

    The difference b - a is safe if it can be represented by the type
    of a and b, i.e. there is no overflow.

    \param a Smallest value
    \param b Greatest value
    \pre a < b
*/
template<class T>
bool difference_is_safe(T a, T b)
{
  assert(a < b);

  if (a < 0 && b < 0)
    return true;

  if (a >= 0 && b >= 0)
    return true;

  assert(a < 0);
  assert(b >= 0);

  if (-a <= std::numeric_limits<T>::max() - b)
    return true;

  return false;
}


/// Dyadic integer representation
template<class T>
class DyadicIntegerRepresentation {

  /// Number of bits
  int _num_bits;

  /// Number of bits for a complete representation
  int _num_bits_complete;

  /// Lower bound of the interval
  T _lower_bound;

  /// Upper bound of the interval
  T _upper_bound;

  /// The the number of bits of a complete representation
  void set_num_bits_complete(T lower_bound, T upper_bound) {
    assert(lower_bound < upper_bound);
    assert(difference_is_safe(lower_bound, upper_bound));

    _num_bits_complete = std::ceil(std::log(upper_bound - lower_bound + 1) / std::log(2));
    assert(_num_bits_complete > 0);
    assert(_num_bits_complete < int(sizeof(T) * 8));
  }

public:

  /// Domain type
  typedef T domain_type;

  /** Constructor.

      The represented interval is [lower_bound..upper_bound].

      \param num_bits Number of bits per integer
      \param lower_bound Lower bound of the interval
      \param upper_bound Upper bound of the interval
  */
  DyadicIntegerRepresentation(T lower_bound, T upper_bound, int num_bits)
    : _num_bits(num_bits)
    , _lower_bound(lower_bound)
    , _upper_bound(upper_bound)
  {
    assert(num_bits > 0);
    assert(num_bits < int(sizeof(T) * 8));

    set_num_bits_complete(lower_bound, upper_bound);
    if (_num_bits > _num_bits_complete)
      _num_bits = _num_bits_complete;
  }

  /** Constructor.

      The represented interval is [lower_bound..upper_bound].

      \param lower_bound Lower bound of the interval
      \param upper_bound Upper bound of the interval
  */
  DyadicIntegerRepresentation(T lower_bound, T upper_bound)
    : _lower_bound(lower_bound)
    , _upper_bound(upper_bound)
  {

    set_num_bits_complete(lower_bound, upper_bound);
    _num_bits = _num_bits_complete;
  }

  /// Size of the representation
  int size() const { return _num_bits; }

  /// Unpack bit vector into a value
  domain_type unpack(const bit_vector_t& bv, int start) {
    std::size_t u = bv_to_size_type(bv, start, start + size());
    if (_num_bits < _num_bits_complete)
      u <<= (_num_bits_complete - _num_bits);
    if (u > size_t(_upper_bound - _lower_bound))
      u = _upper_bound - _lower_bound;
    T result = _lower_bound + T(u);
    assert(result >= _lower_bound);
    if (result > _upper_bound)
      result = _upper_bound;
    return result;
  }

  /// Display
  void display(std::ostream& stream) const {
    stream << "DyadicIntegerRepresentation " << "[" << _lower_bound << ", " << _upper_bound << "] (" << size() << " bits)";
  }

};


} // end of namespace representation
} // end of namespace hnco


#endif
