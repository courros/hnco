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

#ifndef HNCO_REPRESENTATIONS_VALUE_SET_H
#define HNCO_REPRESENTATIONS_VALUE_SET_H

#include <assert.h>

#include <vector>
#include <iostream>             // std::ostream
#include <algorithm>            // std::sort

#include "hnco/util.hh"         // hnco::is_in_range, hnco::join
#include "hnco/bit-vector.hh"

#include "integer.hh"           // hnco::representation::DyadicIntegerRepresentation

namespace hnco {
namespace representation {

/// Value set
template<class T>
class ValueSet {
  /// Values
  std::vector<T> _values;
  /// Index representation
  DyadicIntegerRepresentation<int> _index_representation;
public:
  /// Domain type
  using domain_type = T;
  /**
   * Constructor.
   * @param values Values
   */
  ValueSet(const std::vector<T>& values)
    : _values(values)
    , _index_representation(values.size())
  {
    std::sort(begin(_values), end(_values));
  }
  /// Size of the representation
  int size() const { return _index_representation.size(); }
  /// Unpack bit vector into a value
  domain_type unpack(const bit_vector_t& bv, int start) {
    assert(hnco::is_in_range(start, bv.size()));
    int index = _index_representation.unpack(bv, start);
    assert(hnco::is_in_range(index, _values.size()));
    return _values[index];
  }
  /// Display
  void display(std::ostream& stream) const {
    stream << "ValueSet "
           << "{"
           << hnco::join(_values.begin(), _values.end(), ", ")
           << "} ("
           << size() << " bits)";
  }
};

} // end of namespace representation
} // end of namespace hnco

#endif
