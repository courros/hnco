/* Copyright (C) 2016, 2017, 2018 Arnaud Berny

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

#ifndef HNCO_FUNCTIONS_WALSH_TRANSFORM_H
#define HNCO_FUNCTIONS_WALSH_TRANSFORM_H

#include <assert.h>

#include <vector>

#include "hnco/functions/function.hh"


namespace hnco {
namespace function {


  /** Walsh transform.

      Its expression is of the form

      \f$ f(x) = \sum_u a_u (-1)^{x \cdot u} \f$

      where the sum is over \f$ \{0, 1\}^n \f$ and \f$ x \cdot u =
      \sum_i x_i u_i \f$ is mod 2. The real numbers \f$ a_u \f$ are
      the coefficients of the expansion and the bit vectors \f$ u \f$
      are its feature vectors.
  */
  class WalshTransform {

    /// Function
    Function *_function;

    /// Coefficients
    std::vector<double> _coefficients;

  public:

    /// Constructor
    WalshTransform(Function *function):
      _function(function)
    {
      assert(function);

      std::size_t N = 1 << _function->get_bv_size();
      _coefficients = std::vector<double>(N, 0.0);
    }

    /// Get coefficients
    const std::vector<double>& get_coefficients() { return _coefficients; }

    /** Compute the Walsh transform.

        The first coefficient is set to zero independently of the
        function, which corresponds to removing its average
        value. This is done for the purpose of normalization.
    */
    void compute();

  };


} // end of namespace function
} // end of namespace hnco


#endif
