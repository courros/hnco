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

#ifndef HNCO_ALGORITHMS_DECORATORS_RESTART_H
#define HNCO_ALGORITHMS_DECORATORS_RESTART_H

#include <assert.h>

#include "hnco/algorithms/iterative-algorithm.hh"


namespace hnco {
namespace algorithm {


/** Restart.

    Restart an Algorithm an indefinite number of times. Should be
    used in conjonction with OnBudgetFunction or StopOnMaximum.
*/
class Restart:
    public IterativeAlgorithm {

protected:

  /// Algorithm
  Algorithm *_algorithm;

  /** @name Loop
   */
  ///@{

  /// Initialize
  void init() override {
    _algorithm->set_function(_function);
    _algorithm->set_functions(_functions);
  }

  /// Single iteration
  void iterate() override;

  ///@}

public:

  /// Constructor
  Restart(int n, Algorithm *algorithm):
    IterativeAlgorithm(n),
    _algorithm(algorithm)
  {
    assert(algorithm);
  }

};


} // end of namespace algorithm
} // end of namespace hnco


#endif
