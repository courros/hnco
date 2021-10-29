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

#ifndef HNCO_FUNCTIONS_REPRESENTATIONS_PERMUTATION_REPRESENTATION_H
#define HNCO_FUNCTIONS_REPRESENTATIONS_PERMUTATION_REPRESENTATION_H

#include <assert.h>

#include <vector>
#include <iostream>             // std::ostream
#include <cmath>                // std::log, std::ceil

#include "hnco/bit-vector.hh"
#include "hnco/permutation.hh"  // hnco::permutation_t


namespace hnco {
namespace function {
namespace representation {


/// Permutation representation
class PermutationRepresentation {

public:

  /// Element
  struct Element {
    /// Index
    int index;
    /// Value
    int value;
  };

private:

  /// Elements
  std::vector<Element> _elements;

  /// Number of bits per element
  int _num_bits;

  /// Representation size
  int _representation_size;

public:

  /** Constructor.

      Each element is represented by an integer encoded using
      std::ceil(std::log(num_elements) / std::log(2)) +
      num_additional_bits.

      \param num_elements Number of elements
      \param num_additional_bits Number of additional bits per element
  */
  PermutationRepresentation(int num_elements, int num_additional_bits)
    : _elements(num_elements)
  {
    assert(num_elements > 0);
    assert(num_additional_bits > 0);

    int num_bits_min = std::ceil(std::log(num_elements) / std::log(2));
    _num_bits = num_bits_min + num_additional_bits;
    _representation_size = _elements.size() * _num_bits;
  }

  /// Get number of elements
  int get_num_elements() const { return _elements.size(); }

  /// Size of the representation
  int size() const { return _representation_size; }

  /// Unpack bit vector into a permutation
  void unpack(const bit_vector_t& bv, int start, hnco::permutation_t& permutation) {
    assert(start >= 0);
    assert(permutation.size() == _elements.size());

    for (size_t i = 0; i < _elements.size(); i++) {
      _elements[i].index = i;
      _elements[i].value = bv_to_size_type(bv, start, start + _num_bits);
      start += _num_bits;
    }

    std::sort(_elements.begin(), _elements.end(), [](Element& a, Element& b){ return a.value < b.value; });
    for (size_t i = 0; i < permutation.size(); i++)
      permutation[i] = _elements[i].index;
    assert(hnco::perm_is_valid(permutation));
  }

  /// Display
  void display(std::ostream& stream) const {
    stream << "Permutations of "
           << _elements.size() << " elements represented by "
           << _elements.size() << " * " << _num_bits << " = "
           << size() << " bits" << std::endl;
  }

};


} // end of namespace representation
} // end of namespace function
} // end of namespace hnco


#endif
