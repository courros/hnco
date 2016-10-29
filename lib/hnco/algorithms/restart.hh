/* Copyright (C) 2016 Arnaud Berny

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

#ifndef HNCO_ALGORITHMS_RESTART_H
#define HNCO_ALGORITHMS_RESTART_H

#include <assert.h>

#include "algorithm.hh"


namespace hnco {
namespace algorithm {


  /** Restart.

      Restart any algorithm an indefinite number of times. Should be
      used in conjonction with bounded evaluation.
  */
  class Restart:
    public IterativeAlgorithm {

    /// Algorithm
    Algorithm *_algorithm;
      
    /// Optimize
    void iterate();

  public:

    /// Constructor
    Restart(int n, Algorithm *algorithm):
      IterativeAlgorithm(n),
      _algorithm(algorithm)
    {
      assert(algorithm);
    }

    /// Set function
    void set_function(function::Function *function) {
      assert(function);
      _function = function;
      _algorithm->set_function(function);
    }

  };


} // end of namespace algorithm
} // end of namespace hnco


#endif
