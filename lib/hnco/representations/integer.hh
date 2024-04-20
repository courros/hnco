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

#ifndef HNCO_REPRESENTATIONS_INTEGER_H
#define HNCO_REPRESENTATIONS_INTEGER_H

#include <algorithm>            // std::clamp
#include <cassert>
#include <cmath>                // std::log, std::ceil
#include <iostream>             // std::ostream

#include "hnco/bit-vector.hh"


namespace hnco {
/// Representations
namespace representation {


/**
 * Check whether the difference is safe.
 *
 * The template parameter T must be an integral type such as int or
 * long.
 *
 * The difference b - a is safe if it can be represented by the type
 * of a and b, i.e. there is no overflow.
 *
 * @param a Smallest value
 * @param b Greatest value
 * @pre a < b
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

  /// Size in bits
  int _size;

  /// Exact size required for a given interval
  int _exact_size;

  /// Lower bound of the interval
  T _lower_bound;

  /// Upper bound of the interval
  T _upper_bound;

  /// Set the exact size for a given interval
  void set_exact_size(T lower_bound, T upper_bound) {
    assert(lower_bound < upper_bound);
    assert(difference_is_safe(lower_bound, upper_bound));

    _exact_size = std::ceil(std::log(upper_bound - lower_bound + 1) / std::log(2));
    assert(_exact_size > 0);
    assert(_exact_size < int(sizeof(T) * 8));
  }

public:

  /// Domain type
  using domain_type = T;

  /// %Precision
  struct Precision {

    /// Precison
    int precision;

    /// Constructor
    Precision(int precision): precision(precision) {}
  };

  /**
   * Constructor with given size.
   *
   * The represented interval is [lower_bound..upper_bound].
   *
   * @param lower_bound Lower bound of the interval
   * @param upper_bound Upper bound of the interval
   * @param size Size in bits per integer
   */
  DyadicIntegerRepresentation(T lower_bound, T upper_bound, int size)
    : _size(size)
    , _lower_bound(lower_bound)
    , _upper_bound(upper_bound)
  {
    assert(size > 0);
    assert(size < int(sizeof(T) * 8));

    set_exact_size(lower_bound, upper_bound);
    if (_size > _exact_size)
      _size = _exact_size;
  }

  /**
   * Constructor.
   *
   * The represented interval is [lower_bound..upper_bound].
   *
   * @param lower_bound Lower bound of the interval
   * @param upper_bound Upper bound of the interval
   */
  DyadicIntegerRepresentation(T lower_bound, T upper_bound)
    : _lower_bound(lower_bound)
    , _upper_bound(upper_bound)
  {
    set_exact_size(lower_bound, upper_bound);
    _size = _exact_size;
  }

  /**
   * Constructor with given precision.
   *
   * The represented interval is [lower_bound..upper_bound].
   *
   * @param lower_bound Lower bound of the interval
   * @param upper_bound Upper bound of the interval
   * @param precision %Precision
   */
  DyadicIntegerRepresentation(T lower_bound, T upper_bound, Precision precision)
    : _lower_bound(lower_bound)
    , _upper_bound(upper_bound)
  {
    if (precision.precision < 1)
      throw std::runtime_error
        ("DyadicIntegerRepresentation::DyadicIntegerRepresentation: Bad precision: " + std::to_string(precision.precision));

    set_exact_size(lower_bound, upper_bound);
    _size = std::ceil(_exact_size - std::log(precision.precision) / std::log(2));
    _size = std::clamp(_size, 1, _exact_size);
  }

  /**
   * Constructor.
   * The represented interval is [0..n-1].
   * @param n Number of elements
   */
  DyadicIntegerRepresentation(T n)
    : _lower_bound(0)
    , _upper_bound(n - 1)
  {
    assert(n > 0);
    set_exact_size(_lower_bound, _upper_bound);
    _size = _exact_size;
  }

  /// Size of the representation
  int size() const { return _size; }

  /// Unpack bit vector into a value
  domain_type unpack(const bit_vector_t& bv, int start) {
    std::size_t u = bv_to_size_type(bv, start, start + size());
    if (_size < _exact_size)
      u <<= (_exact_size - _size);
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
