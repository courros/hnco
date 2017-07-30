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


  /** (1+1) EA.

      (1+1) EA is implemented as a RandomLocalSearch with a Binomial
      neighborhood and infinite patience. Thus it does derive from
      IterativeAlgorithm.
  */
  class OnePlusOneEa:
    public Algorithm {

    /// Neighborhood
    neighborhood::Binomial _neighborhood;

    /// Random local search
    RandomLocalSearch _rls;

  public:

    /** Constructor.

        \param n Size of bit vectors

        _mutation_probability is initialized to 1 / n.
    */
    OnePlusOneEa(int n):
      Algorithm(n),
      _neighborhood(n),
      _rls(n, &_neighborhood),
      _mutation_probability(1 / double(n)) {}

    /// Set function
    void set_function(function::Function *function) {
      assert(function);
      _rls.set_function(function);
    }

    /// Initialization
    void init() {
      _neighborhood.set_mutation_probability(_mutation_probability);
      _rls._num_iterations              = _num_iterations;
      _rls._incremental_evaluation      = _incremental_evaluation;
      _rls._patience                    = 0;
      _rls.init();
    }

    /// Maximize
    void maximize() { _rls.maximize(); }

    /// Solution
    const point_value_t& get_solution() { return _rls.get_solution(); }

    /** @name Parameters
     */
    ///@{

    /** Number of iterations.
        _num_iterations <= 0 means indefinite */
    int _num_iterations = 0;

    /// Incremental evaluation
    bool _incremental_evaluation = false;

    /// Mutation probability
    double _mutation_probability;

    ///@}

  };


} // end of namespace algorithm
} // end of namespace hnco


#endif
