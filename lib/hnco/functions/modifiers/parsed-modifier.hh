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

#ifndef HNCO_FUNCTIONS_MODIFIERS_PARSED_MODIFIER_H
#define HNCO_FUNCTIONS_MODIFIERS_PARSED_MODIFIER_H

#include <assert.h>

#include "fparser/fparser.hh"

#include "modifier.hh"

namespace hnco {
namespace function {
namespace modifier {

/** Parsed modifier.

    Let f be the original function. Then the modified function is
    equivalent to \f$g\circ f\f$, where g is a real function defined
    by an expression \f$g(x)\f$ provided as a string.
*/
class ParsedModifier:
    public Modifier {

  /// Function parser
  FunctionParser _fparser;

  /// Array of values
  double _values[1];

public:

  /** Constructor.

      \param function Decorated function
      \param expression Expression to parse
  */
  ParsedModifier(Function *function, std::string expression);

  /** @name Information about the function
   */
  ///@{

  /// Get bit vector size
  int get_bv_size() const override { return _function->get_bv_size(); }

  ///@}


  /** @name Evaluation
   */
  ///@{

  /// Evaluate a bit vector
  double evaluate(const bit_vector_t&) override;

  ///@}

};


} // end of namespace modifier
} // end of namespace function
} // end of namespace hnco


#endif
