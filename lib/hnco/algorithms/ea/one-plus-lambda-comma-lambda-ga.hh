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

#ifndef HNCO_ALGORITHMS_EA_ONE_PLUS_LAMDDA_COMMA_LAMBDA_GA
#define HNCO_ALGORITHMS_EA_ONE_PLUS_LAMDDA_COMMA_LAMBDA_GA

#include <assert.h>

#include <random>

#include "hnco/algorithms/algorithm.hh"
#include "hnco/algorithms/population.hh"
#include "hnco/neighborhoods/neighborhood.hh"
#include "hnco/random.hh"

#include "crossover.hh"


namespace hnco {
namespace algorithm {


  /** (1+(lambda,lambda)) genetic algorithm.

   */
  class OnePlusLambdaCommaLambdaGa:
    public IterativeAlgorithm {

    /// Offsprings
    Population _offsprings;

    /// Radius distribution
    std::binomial_distribution<int> _radius_dist;

    /// Mutation operator
    neighborhood::HammingSphere _mutation;

    /// Parent
    bit_vector_t _parent;

    /// Biased crossover
    BiasedCrossover _crossover;

    /// Single iteration
    void iterate();

  public:

    /** Constructor.

        \param n Size of bit vectors
        \param lambda Offspring population size
    */
    OnePlusLambdaCommaLambdaGa(int n, int lambda):
      IterativeAlgorithm(n),
      _offsprings(lambda, n),
      _mutation(n, 1),
      _parent(n)
    {
      assert(n > 0);
      assert(lambda > 0);
      assert(lambda <= n);

      _mutation_probability = double(lambda) / double(n);
      _crossover_bias = 1 / double(lambda);
    }

    /// Initialization
    void init();

    /** @name Parameters
     */
    ///@{

    /// Mutation probability
    double _mutation_probability;

    /// Crossover bias
    double _crossover_bias;

    ///@}

  };


} // end of namespace algorithm
} // end of namespace hnco


#endif
