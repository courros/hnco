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


namespace hnco {
namespace algorithm {


  /// (1+1)-EA
  class OnePlusOneEa:
    public IterativeAlgorithm {

    /// Neighborhood
    neighborhood::Binomial _neighborhood;

    /// Single iteration
    void iterate();

  public:

    /// Constructor
    OnePlusOneEa(int n):
      IterativeAlgorithm(n),
      _neighborhood(n) {}

    /// Set mutation probability
    void set_mutation_probability(double p) { _neighborhood.set_mutation_probabiity(p); }

    /// Random initialization
    void init();

    /// Solution
    const bit_vector_t& get_solution() { return _neighborhood.get_origin(); }

  };


} // end of namespace algorithm
} // end of namespace hnco


#endif
