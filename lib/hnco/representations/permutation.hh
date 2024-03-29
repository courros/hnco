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

#ifndef HNCO_REPRESENTATIONS_PERMUTATION_H
#define HNCO_REPRESENTATIONS_PERMUTATION_H

#include <assert.h>

#include <vector>
#include <iostream>             // std::ostream
#include <cmath>                // std::log, std::ceil

#include "hnco/bit-vector.hh"
#include "hnco/permutation.hh"  // hnco::permutation_t


namespace hnco {
namespace representation {


/// Permutation representation
class PermutationRepresentation {

  /// Values to be sorted
  std::vector<int> _values;

  /// Element size in bits
  int _element_size;

  /// Size in bits
  int _size;

public:

  /**
   * Constructor.
   *
   * Each element is represented by an integer encoded using
   * std::ceil(std::log(num_elements) / std::log(2)) +
   * num_additional_bits.
   *
   * @param num_elements Number of elements
   * @param num_additional_bits Number of additional bits per element
   */
  PermutationRepresentation(int num_elements, int num_additional_bits)
    : _values(num_elements)
  {
    assert(num_elements > 0);
    assert(num_additional_bits > 0);

    int exact_element_size = std::ceil(std::log(num_elements) / std::log(2));
    _element_size = exact_element_size + num_additional_bits;
    _size = _values.size() * _element_size;
  }

  /// Get number of elements
  int get_num_elements() const { return _values.size(); }

  /// Size of the representation
  int size() const { return _size; }

  /// Unpack bit vector into a permutation
  void unpack(const bit_vector_t& bv, int start, hnco::permutation_t& permutation) {
    assert(start >= 0);
    assert(permutation.size() == _values.size());

    for (size_t i = 0; i < _values.size(); i++) {
      permutation[i] = i;
      _values[i] = bv_to_size_type(bv, start, start + _element_size);
      start += _element_size;
    }
    perm_shuffle(permutation);
    auto compare = [this](int i, int j){ return _values[i] < _values[j]; };
    std::sort(permutation.begin(), permutation.end(), compare);

    assert(hnco::perm_is_valid(permutation));
  }

  /// Display
  void display(std::ostream& stream) const {
    stream << "PermutationRepresentation: "
           << _values.size() << " elements * "
           << _element_size << " bits ("
           << _size << " bits)";
  }

};


} // end of namespace representation
} // end of namespace hnco


#endif
