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

#ifndef HNCO_FUNCTIONS_PERMUTATION_FUNCTION_ADAPTER_H
#define HNCO_FUNCTIONS_PERMUTATION_FUNCTION_ADAPTER_H

#include <assert.h>

#include "hnco/exception.hh"
#include "hnco/representations/permutation.hh"

#include "function.hh"


namespace hnco {
namespace function {

/** Permutation function adapter.

    The purpose of this class is to build a regular hnco function from
    an arbitrary function over permutations. This is achieved using a
    permutation representation.
*/
template<class Fn>
class PermutationFunctionAdapter: public Function {

  /// Permutation function
  Fn *_function;

  /// Permutation representation
  representation::PermutationRepresentation _representation;

  /// Permutation
  permutation_t _permutation;

  /// Unpack a bit vector into a permutation
  void unpack(const bit_vector_t& bv) {
    assert(int(bv.size()) == _representation.size());
    _representation.unpack(bv, 0, _permutation);
  }

public:

  /** Constructor.

      \param fn Multivariate function
      \param rep Permutation representation
  */
  PermutationFunctionAdapter(Fn *fn, representation::PermutationRepresentation rep)
    : _function(fn)
    , _representation(rep)
  {
    assert(fn);

    _permutation.resize(fn->get_num_elements());
    if (_representation.get_num_elements() != _function->get_num_elements())
      throw std::runtime_error("PermutationFunctionAdapter::PermutationFunctionAdapter: Function and representation must have the same number of elements");
  }

  /// Get bit vector size
  int get_bv_size() const override { return _representation.size(); }

  /// Evaluate
  double evaluate(const bit_vector_t& bv) override {
    unpack(bv);
    return _function->evaluate(_permutation);
  }

  /// Display
  void display(std::ostream& stream) const override {
    _function->display(stream);
    _representation.display(stream);
  }

  /// Describe a bit vector
  void describe(const bit_vector_t& bv, std::ostream& stream) override {
    unpack(bv);
    _function->describe(_permutation, stream);
  }

};

} // end of namespace function
} // end of namespace hnco


#endif
