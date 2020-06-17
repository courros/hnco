/* Copyright (C) 2016, 2017, 2018, 2019, 2020 Arnaud Berny

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
namespace function {
/// Real multivariate functions
namespace real {


  /// Real multivariate function adapter
  class RealMultivariateFunctionAdapter:
    public hnco::function::Function {

    /// Real representation
    RealRepresentation *_representation;

    /// Real multivariate function
    RealMultivariateFunction *_function;

    /// Real vector
    std::vector<double> _rv;

    /// Convert a bit vector
    void convert(const bit_vector_t& x);

  public:

    /** Constructor.

        \param rep Real representation
        \param fn Real multivariate function
    */
    RealMultivariateFunctionAdapter(RealRepresentation *rep,
                                    RealMultivariateFunction *fn):
      _representation(rep),
      _function(fn)
    {
      assert(rep);
      assert(fn);

      _rv = std::vector<double>(fn->get_dimension());
    }

    /** @name Information about the function
     */
    ///@{

    /// Get bit vector size
    int get_bv_size() { return _function->get_dimension() * _representation->size(); }

    ///@}


    /** @name Evaluation
     */
    ///@{

    /// Evaluate a bit vector
    double evaluate(const bit_vector_t& x);

    ///@}


    /** @name Display
     */
    ///@{

    /// Describe a bit vector
    void describe(const bit_vector_t& x, std::ostream& stream);

    ///@}

  };


} // end of namespace real
} // end of namespace function
} // end of namespace hnco


#endif
