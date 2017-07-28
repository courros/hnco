/* Copyright (C) 2016, 2017 Arnaud Berny

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

#ifndef HNCO_ALGORITHMS_EA_ONE_PLUS_ONE
#define HNCO_ALGORITHMS_EA_ONE_PLUS_ONE

#include "hnco/algorithms/algorithm.hh"
#include "hnco/algorithms/ls/neighborhood.hh"
#include "hnco/algorithms/ls/random-local-search.hh"


namespace hnco {
namespace algorithm {


  /// (1+1) EA
  class OnePlusOneEa:
    public Algorithm {

  public:

    /// Neighborhood
    neighborhood::Binomial _neighborhood;

    /// Random local search
    NonStrictRandomLocalSearch _rls;

    /// Constructor
    OnePlusOneEa(int n):
      Algorithm(n),
      _neighborhood(n),
      _rls(n, &_neighborhood) {}

    /// Set function
    void set_function(function::Function *function) {
      assert(function);
      _rls.set_function(function);
    }

    /// Initialization
    void init();

    /// Maximize
    void maximize();

  };


} // end of namespace algorithm
} // end of namespace hnco


#endif
