/* Copyright (C) 2016, 2017, 2018, 2019, 2020, 2021, 2022, 2023 Arnaud Berny

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

#ifndef HNCO_ALGORITHMS_GOMEA_HNCO_FITNESS_H
#define HNCO_ALGORITHMS_GOMEA_HNCO_FITNESS_H

#include <assert.h>

#include "hnco/functions/function.hh"

#include "gomea/src/fitness/bbo_fitness.hpp"

namespace hnco {
namespace algorithm {
namespace gomea {

/// Fitness for HNCO functions
class HncoFitness:
    public ::gomea::fitness::BBOFitnessFunction_t<char> {

  /// HNCO function
  hnco::function::Function *_function;

  /// Argument of HNCO function
  hnco::bit_vector_t _bv;

public:

  /// Constructor
  HncoFitness(hnco::function::Function *function)
    : BBOFitnessFunction_t<char>(function->get_bv_size())
    , _function(function)
  {
    _bv = bit_vector_t(_function->get_bv_size());
  }

  /// Evaluate a bit vector
  double objectiveFunction(int objective_index, ::gomea::vec_t<char>& variables) override {
    assert(variables.size() == _bv.size());
    for (size_t i = 0; i < _bv.size(); ++i)
      _bv[i] = variables[i];
    return _function->evaluate(_bv);
  }

};

} // end of namespace gomea
} // end of namespace algorithm
} // end of namespace hnco


#endif
