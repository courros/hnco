/* Copyright (C) 2016, 2017, 2018, 2019 Arnaud Berny

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

#ifndef HNCO_FUNCTIONS_DECORATORS_EXPRESSION_MODIFIER_H
#define HNCO_FUNCTIONS_DECORATORS_EXPRESSION_MODIFIER_H

#include <assert.h>

#include "fparser/fparser.hh"

#include "function-modifier.hh"

namespace hnco {
namespace function {

  /** Expression modifier.

      Let f be the original function. Then the modified function is
      equivalent to \f$g\circ f\f$, where g is a real function defined
      by an expression \f$g(x)\f$ provided as a string.
  */
  class ExpressionModifier:
    public FunctionModifier {

    /// Function parser
    FunctionParser _fparser;

    /// Array of values
    double _values[1];

  public:

    /** Constructor.

        \param function Decorated function
        \param expression Expression to parse
    */
    ExpressionModifier(Function *function, std::string expression);

    /** @name Information about the function
     */
    ///@{

    /// Get bit vector size
    int get_bv_size() { return _function->get_bv_size(); }

    ///@}


    /** @name Evaluation
     */
    ///@{

    /// Evaluate a bit vector
    double eval(const bit_vector_t&);

    ///@}

  };


} // end of namespace function
} // end of namespace hnco


#endif
