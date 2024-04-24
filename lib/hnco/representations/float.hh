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

#ifndef HNCO_REPRESENTATIONS_FLOAT_H
#define HNCO_REPRESENTATIONS_FLOAT_H

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
namespace representation {


/// Dyadic float representation
template<class T>
class DyadicFloatRepresentation {

  /// Lengths of dyadic intervals
  std::vector<T> _lengths;

  /// Lower bound of the interval
  T _lower_bound;

  /// Length of the interval
  T _length;

  /// Affine transformation
  T affine_transformation(T x) { return _lower_bound + _length * x; }

  /**
   * Compute lengths.
   *
   * @param size Size in bits per float number
   */
  void compute_lengths(int size)
  {
    assert(size > 0);

    _lengths = std::vector<T>(size);
    T x = 0.5;
    for (size_t i = 0; i < _lengths.size(); i++) {
      _lengths[i] = x;
      x /= 2;
    }
  }

public:

  /// Domain type
  using domain_type = T;

  /**
   * Constructor.
   *
   * The represented interval is [lower_bound, upper_bound).
   *
   * @param lower_bound Lower bound of the interval
   * @param upper_bound Upper bound of the interval
   * @param size Size in bits per float number
   */
  DyadicFloatRepresentation(T lower_bound, T upper_bound, int size)
    : _lower_bound(lower_bound)
    , _length(upper_bound - lower_bound)
  {
    assert(lower_bound < upper_bound);
    assert(size > 0);

    compute_lengths(size);
  }

  /**
   * Constructor.
   *
   * The represented interval is [lower_bound, upper_bound).
   *
   * @param lower_bound Lower bound of the interval
   * @param upper_bound Upper bound of the interval
   * @param precision Precision
   */
  DyadicFloatRepresentation(T lower_bound, T upper_bound, T precision)
    : _lower_bound(lower_bound)
    , _length(upper_bound - lower_bound)
  {
    assert(lower_bound < upper_bound);
    assert(precision > 0);

    int size = std::ceil(std::log(_length / precision) / std::log(2));
    compute_lengths(size);
  }

  /// Size of the representation
  int size() const { return _lengths.size(); }

  /// Unpack bit vector into a value
  domain_type unpack(const bit_vector_t& bv, int start) {
    assert(hnco::is_in_range(start, bv.size()));

    int stop = start + size();
    assert(hnco::is_in_range(stop, start + 1, bv.size() + 1));

    T result = 0;
    for (int i = start, offset = 0; i < stop; i++, offset++) {
      assert(hnco::is_in_range(i, bv.size()));
      assert(hnco::is_in_range(offset, _lengths.size()));
      if (bv[i])
        result += _lengths[offset];
    }
    return affine_transformation(result);
  }

  /// Display
  void display(std::ostream& stream) const {
    stream << "DyadicFloatRepresentation " << "[" << _lower_bound << ", " << (_lower_bound + _length) << ") (" << size() << " bits)";
  }

};


} // end of namespace representation
} // end of namespace hnco


#endif
