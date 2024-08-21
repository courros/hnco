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

#ifndef HNCO_FUNCTIONS_MULTIOBJECTIVE_FUNCTION_ADAPTER_H
#define HNCO_FUNCTIONS_MULTIOBJECTIVE_FUNCTION_ADAPTER_H

#include <cassert>
#include <numeric>              // std::accumulate

#include "hnco/multiobjective/functions/function.hh"

#include "function.hh"

namespace hnco {
namespace function {

/**
 * Multiobjective function adapter.
 * The purpose of this class is to build a function from an arbitrary
 * multiobjective function. This is achieved using a composition:
 * - Multiobjective function: bit vector -> [double]
 * - Linear combination: [double] -> double
 * 
 */
class MultiobjectiveFunctionAdapter: public Function {
  /// Multiobjective function
  multiobjective::function::Function *_function;
  /// Output value of the multiobjective function
  multiobjective::function::value_t _value;

public:
  /**
   * Constructor.
   * @param fn Multiobjective function
   */
  MultiobjectiveFunctionAdapter(multiobjective::function::Function *fn)
    : _function(fn)
  {
    assert(fn);
    _value.resize(_function->get_output_size());
  }
  /**
   * @name Properties
   */
  ///@{
  /// Get bit vector size
  int get_bv_size() const override { return _function->get_bv_size(); }
  ///@}
  /**
   * @name Evaluation
   */
  ///@{
  /// Evaluate
  double evaluate(const bit_vector_t& bv) override {
    _function->evaluate(bv, _value);
    return std::accumulate(begin(_value), end(_value), double(0));
  }
  ///@}
  /**
   * @name Display
   */
  ///@{
  /// Display
  void display(std::ostream& stream) const override { _function->display(stream); }
  /// Describe a bit vector
  void describe(const bit_vector_t& bv, std::ostream& stream) override { _function->describe(bv, stream); }
  ///@}
};

} // end of namespace function
} // end of namespace hnco


#endif
