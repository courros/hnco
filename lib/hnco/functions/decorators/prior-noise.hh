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

#ifndef HNCO_FUNCTIONS_DECORATORS_PRIOR_NOISE_H
#define HNCO_FUNCTIONS_DECORATORS_PRIOR_NOISE_H

#include <assert.h>

#include "hnco/neighborhoods/neighborhood.hh"

#include "function-modifier.hh"


namespace hnco {
namespace function {


  /// Prior noise
  class PriorNoise:
    public FunctionModifier {

    /// Neighborhood
    neighborhood::Neighborhood *_neighborhood;

    /// Noisy bit vector
    bit_vector_t _noisy_bv;

  public:

    /// Constructor
    PriorNoise(Function *fn, neighborhood::Neighborhood *nh):
      FunctionModifier(fn),
      _neighborhood(nh)
    {
      assert(fn);
      assert(nh);
      _noisy_bv.resize(fn->get_bv_size());
    }

    /** @name Information about the function
     */
    ///@{

    /// Get bit vector size
    int get_bv_size() { return _function->get_bv_size(); }

    /** Get the global maximum.

        Delegation is questionable here.
    */
    double get_maximum() { return _function->get_maximum(); }

    /** Check for a known maximum.

        Delegation is questionable here.
    */
    bool has_known_maximum() { return _function->has_known_maximum(); }

    /** Check whether the function provides incremental evaluation.
        \return false */
    bool provides_incremental_evaluation() { return false; }

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
