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

#ifndef HNCO_REPRESENTATIONS_CATEGORICAL_H
#define HNCO_REPRESENTATIONS_CATEGORICAL_H

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
  using domain_type = std::size_t;

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
    stream << "LinearCategoricalRepresentation (" << _num_categories << " values) (" << _nrows << " * " << _ncols << " bits)";
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
  using domain_type = std::size_t;

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
    stream << "IntegerCategoricalRepresentation (" << _num_categories << " values) (" << _num_bits << " bits)";
  }

};


} // end of namespace representation
} // end of namespace hnco


#endif
