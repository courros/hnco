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

#ifndef HNCO_FUNCTIONS_REAL_REAL_MULTIVARIATE_FUNCTION_ADAPTER_H
#define HNCO_FUNCTIONS_REAL_REAL_MULTIVARIATE_FUNCTION_ADAPTER_H

#include "hnco/functions/function.hh"

#include "real-representation.hh"
#include "real-multivariate-function.hh"


namespace hnco {
namespace hnco {
namespace real {


  /// Real multivariate function adapter
  class RealMultivariateFunctionAdapter:
    public Function {

  public:

    /** @name Information about the function
     */
    ///@{

    /// Get bit vector size
    int get_bv_size();

    ///@}


    /** @name Evaluation
     */
    ///@{

    /// Evaluate a bit vector
    double eval(const bit_vector_t&);

    ///@}


    /** @name Display
     */
    ///@{

    /// Describe a bit vector
    void describe(const bit_vector_t& x, std::ostream& stream) {
      assert(int(x.size()) == get_bv_size());

      bv_display(x, stream);
      stream << std::endl;
    }

    ///@}

  };


} // end of namespace real
} // end of namespace function
} // end of namespace hnco


#endif
