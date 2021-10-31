/* Copyright (C) 2016, 2017, 2018, 2019, 2020, 2021 Arnaud Berny

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

#ifndef HNCO_FUNCTIONS_REPRESENTATIONS_REPRESENTATION_H
#define HNCO_FUNCTIONS_REPRESENTATIONS_REPRESENTATION_H

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
namespace function {
/// Representations
namespace representation {


/// Dyadic real representation
template<class T>
class DyadicRealRepresentation {

  /// Lengths of dyadic intervals
  std::vector<T> _lengths;

  /// Lower bound of the interval
  T _lower_bound;

  /// Length of the interval
  T _length;

  /// Affine transformation
  T affine_transformation(T x) { return _lower_bound + _length * x; }

  /** Compute lengths.

      \param num_bits Number of bits per real number
  */
  void compute_lengths(int num_bits)
  {
    assert(num_bits > 0);

    _lengths = std::vector<T>(num_bits);
    T x = 0.5;
    for (size_t i = 0; i < _lengths.size(); i++) {
      _lengths[i] = x;
      x /= 2;
    }
  }

public:

  /// Domain type
  typedef T domain_type;

  /** Constructor.

      The represented interval is [lower_bound, upper_bound).

      \param lower_bound Lower bound of the interval
      \param upper_bound Upper bound of the interval
      \param num_bits Number of bits per real number
  */
  DyadicRealRepresentation(T lower_bound, T upper_bound, int num_bits)
    : _lower_bound(lower_bound)
    , _length(upper_bound - lower_bound)
  {
    assert(lower_bound < upper_bound);
    assert(num_bits > 0);

    compute_lengths(num_bits);
  }

  /** Constructor.

      The represented interval is [lower_bound, upper_bound).

      \param lower_bound Lower bound of the interval
      \param upper_bound Upper bound of the interval
      \param precision Precision
  */
  DyadicRealRepresentation(T lower_bound, T upper_bound, T precision)
    : _lower_bound(lower_bound)
    , _length(upper_bound - lower_bound)
  {
    assert(lower_bound < upper_bound);
    assert(precision > 0);

    int num_bits = std::ceil(std::log(_length / precision) / std::log(2));
    compute_lengths(num_bits);
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
    stream
      << "[" << _lower_bound << ", "
      << (_lower_bound + _length) << "] (" << size() << " bits)" << std::endl;
  }

};


/// Dyadic complex representation
template<class T>
class DyadicComplexRepresentation {

  /// Representation of the real part
  DyadicRealRepresentation<T> _real_part;

  /// Representation of the imaginary part
  DyadicRealRepresentation<T> _imaginary_part;

public:

  /// Domain type
  typedef std::complex<T> domain_type;

  /** Constructor.

      \param lower_bound_re Lower bound of the real part
      \param upper_bound_re Upper bound of the real part
      \param num_bits_re Number of bits to represent the real part
      \param lower_bound_im Lower bound of the imaginary part
      \param upper_bound_im Upper bound of the imaginary part
      \param num_bits_im Number of bits to represent the imaginary part
  */
  DyadicComplexRepresentation(T lower_bound_re, T upper_bound_re, int num_bits_re, T lower_bound_im, T upper_bound_im, int num_bits_im)
    : _real_part(lower_bound_re, upper_bound_re, num_bits_re)
    , _imaginary_part(lower_bound_im, upper_bound_im, num_bits_im)
  {
    assert(num_bits_re > 0);
    assert(lower_bound_re < upper_bound_re);
    assert(num_bits_im > 0);
    assert(lower_bound_im < upper_bound_im);
  }

  /** Constructor.

      \param lower_bound Lower bound of both real and imaginary parts
      \param upper_bound Upper bound of both real and imaginary parts
      \param num_bits Number of bits to represent both real and imaginary parts
  */
  DyadicComplexRepresentation(T lower_bound, T upper_bound, int num_bits)
    : DyadicComplexRepresentation(lower_bound, upper_bound, num_bits, lower_bound, upper_bound, num_bits)
  {}

  /// Size of the representation
  int size() const { return _real_part.size() + _imaginary_part.size(); }

  /// Unpack bit vector into a value
  domain_type unpack(const bit_vector_t& bv, int start) {
    assert(hnco::is_in_range(start, bv.size()));
    T re = _real_part.unpack(bv, start);
    start += _real_part.size();
    T im = _imaginary_part.unpack(bv, start);
    return std::complex<T>(re, im);
  }

  /// Display
  void display(std::ostream& stream) const {
    _real_part.display(stream);
    _imaginary_part.display(stream);
  }

};


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

      \param num_bits Number of bits per real
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
    stream
      << "[" << _lower_bound << ", " << _upper_bound << "] (" << size() << " bits)" << std::endl;
  }

};


/// Linear categorical representation
class LinearCategoricalRepresentation {

  /// Number of categories
  int _num_categories;

  /// Number of rows
  int _nrows;

  /// Number of columns
  int _ncols;

  /// Linear code as a bit matrix
  bit_matrix_t _A;

  /// Output category
  bit_vector_t _y;

  /// Input bit vector
  bit_vector_t _x;

public:

  /// Domain type
  typedef std::size_t domain_type;

  /** Constructor.

      \param num_categories Number of categories
  */
  LinearCategoricalRepresentation(int num_categories)
    : _num_categories(num_categories)
  {
    assert(num_categories > 0);
    std::bitset<8 * sizeof(unsigned)> b(num_categories);

    // Most significant bit
    size_t index = b.size();
    while (index > 0) {
      index--;
      if (b.test(index))
        break;
    }
    assert(index < b.size());

    // Not a power of 2
    if (b.count() != 1) {
      index++;
      assert(index < b.size());
      b.reset();
      b.set(index);
    }

    // Set the dimensions of matrix _A, vector _x, and vector _y
    _nrows = index;
    _ncols = b.to_ulong();
    _ncols--;
    assert(_ncols > 0);
    _A = bm_rectangular(_nrows, _ncols);
    _x = bit_vector_t(_ncols);
    _y = bit_vector_t(_nrows);

    // Fill matrix _A
    HypercubeIterator iterator(_nrows);
    assert(iterator.has_next());
    _y = iterator.next();
    assert(bv_is_zero(_y));
    int j = 0;
    while (iterator.has_next()) {
      _y = iterator.next();
      assert(j < _ncols);
      bm_set_column(_A, j, _y);
      j++;
    }
  }

  /// Size of the representation
  int size() const { return _ncols; }

  /// Unpack bit vector into a category
  domain_type unpack(const bit_vector_t& bv, int start) {
    assert(hnco::is_in_range(start, bv.size()));
    const int stop = start + size();
    assert(hnco::is_in_range(stop, start + 1, bv.size() + 1));
    for (int src = start, dest = 0; src < stop; src++, dest++)
      _x[dest] = bv[src];
    bm_multiply(_y, _A, _x);
    assert(_num_categories > 0);
    return bv_to_size_type(_y) % std::size_t(_num_categories);
  }

  /// Display
  void display(std::ostream& stream) const {
    stream << "Categorical values (" << _num_categories
           << "), input = " << _ncols
           << " bits, output = " << _nrows
           << " bits" << std::endl;
  }

};


/// Integer categorical representation
class IntegerCategoricalRepresentation {

  /// Number of categories
  int _num_categories;

  /// Number of bits
  int _num_bits;

public:

  /// Domain type
  typedef std::size_t domain_type;

  /** Constructor.
   *
   * \param num_categories Number of categories
   */
  IntegerCategoricalRepresentation(int num_categories)
    : _num_categories(num_categories)
  {
    assert(num_categories > 0);
    std::bitset<8 * sizeof(std::size_t)> b(num_categories);
    // Look for most significant bit
    size_t index = b.size();
    while (index > 0) {
      index--;
      if (b.test(index))
        break;
    }
    assert(index < b.size());
    // Not a power of 2
    if (b.count() != 1)
      index++;
    _num_bits = index;
  }

  /// Size of the representation
  int size() const { return _num_bits; }

  /// Unpack bit vector into a category
  domain_type unpack(const bit_vector_t& bv, int start) {
    const int stop = start + size();
    assert(hnco::is_in_range(start, bv.size()));
    assert(hnco::is_in_range(stop, start + 1, bv.size() + 1));
    assert(_num_categories > 0);
    return bv_to_size_type(bv, start, stop) % std::size_t(_num_categories);
  }

  /// Display
  void display(std::ostream& stream) const {
    stream << "Categorical values (" << _num_categories
           << ") represented by " << _num_bits << " bits" << std::endl;
  }
};


} // end of namespace representation
} // end of namespace function
} // end of namespace hnco


#endif
