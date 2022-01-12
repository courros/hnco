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

#ifndef HNCO_ALGORITHMS_FAST_EFFICIENT_P3_HNCO_EVALUATOR_H
#define HNCO_ALGORITHMS_FAST_EFFICIENT_P3_HNCO_EVALUATOR_H

#include <assert.h>

#include "hnco/functions/function.hh"

#include "fast-efficient-p3/Evaluation.h"


namespace hnco {
namespace algorithm {
namespace fast_efficient_p3 {

/// Evaluator for HNCO functions
class HncoEvaluator:
    public Evaluator {

  /// HNCO function
  hnco::function::Function *_function;

  /// Argument of HNCO function
  hnco::bit_vector_t _bv;

public:

  /// Constructor
  HncoEvaluator(hnco::function::Function *function):
    _function(function)
  {
    assert(function);

    _bv = bit_vector_t(_function->get_bv_size());
  }

  /// Evaluate a bit vector
  float evaluate(const std::vector<bool> & x) {
    assert(x.size() == _bv.size());

    bv_from_vector_bool(_bv, x);
    return _function->evaluate(_bv);
  }

};

} // end of namespace fast_efficient_p3
} // end of namespace algorithm
} // end of namespace hnco


#endif
