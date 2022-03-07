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

#ifndef HNCO_MULTIOBJECTIVE_FUNCTIONS_FUNCTION_H
#define HNCO_MULTIOBJECTIVE_FUNCTIONS_FUNCTION_H

#include <iostream>

#include "hnco/bit-vector.hh"

#include "value.hh"


namespace hnco {

/// Multiobjective optimization
namespace multiobjective {

/// Functions defined on bit vectors
namespace function {


/// %Function
class Function {

public:

  /// Destructor
  virtual ~Function() {}

  /** @name Information about the function
   */
  ///@{

  /// Get bit vector size
  virtual int get_bv_size() const = 0;

  /// Get output size (number of objectives)
  virtual int get_output_size() const = 0;

  ///@}


  /** @name Evaluation
   */
  ///@{

  /** Evaluate a bit vector.

      This member function is not declared const and is not supposed
      to be thread-safe. In particular, in order to evaluate a bit
      vector, it might require some data member to store temporary
      results. In case of parallel evaluation, there should be a copy
      of the function per thread, as is done in
      Population::evaluate_in_parallel().

      \param bv Bit vector to evaluate
      \param value Output value
  */
  virtual void evaluate(const bit_vector_t& bv, value_t& value) = 0;

  ///@}


  /** @name Display
   */
  ///@{

  /// Display
  virtual void display(std::ostream& stream) const { stream << "Function" << std::endl; }

  /** Describe a bit vector.

      The member function describe() is not declared const for the
      same reason evaluate() is not: it might need to decode the given
      bit vector hence use some pre-allocated memory buffer.
  */
  virtual void describe(const bit_vector_t& x, std::ostream& stream) {
    bv_display(x, stream);
    stream << std::endl;
  }

  ///@}

};


} // end of namespace function
} // end of namespace multiobjective
} // end of namespace hnco


#endif
