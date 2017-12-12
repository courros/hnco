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

#ifndef HNCO_ALGORITHMS_EA_MU_COMMA_LAMBDA_H
#define HNCO_ALGORITHMS_EA_MU_COMMA_LAMBDA_H

#include "hnco/algorithms/algorithm.hh"
#include "hnco/algorithms/population.hh"
#include "hnco/neighborhoods/neighborhood.hh"
#include "hnco/random.hh"


namespace hnco {
namespace algorithm {


  /// (mu,lambda) EA
  class MuCommaLambdaEa:
    public IterativeAlgorithm {

    /// Parents
    Population _parents;

    /// Offsprings
    Population _offsprings;

    /// Mutation operator
    neighborhood::BernoulliProcess _mutation;

    /// Select parent
    std::uniform_int_distribution<int> _select_parent;

    /// Single iteration
    void iterate();

  public:

    /// Constructor
    MuCommaLambdaEa(int n, int mu, int lambda):
      IterativeAlgorithm(n),
      _parents(mu, n),
      _offsprings(lambda, n),
      _mutation(n),
      _select_parent(0, mu - 1),
      _mutation_probability(1 / double(n)) {};

    /// Initialization
    void init();

    /** @name Parameters
     */
    ///@{

    /// Mutation probability
    double _mutation_probability;

    ///@}

  };


} // end of namespace algorithm
} // end of namespace hnco


#endif
