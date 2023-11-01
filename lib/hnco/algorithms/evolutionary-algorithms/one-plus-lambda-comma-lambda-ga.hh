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

#ifndef HNCO_ALGORITHMS_EA_ONE_PLUS_LAMBDA_COMMA_LAMBDA_GA
#define HNCO_ALGORITHMS_EA_ONE_PLUS_LAMBDA_COMMA_LAMBDA_GA

#include <assert.h>

#include <random>

#include "hnco/algorithms/iterative-algorithm.hh"
#include "hnco/algorithms/population.hh"
#include "hnco/neighborhoods/neighborhood.hh"
#include "hnco/random.hh"

#include "crossover.hh"


namespace hnco {
namespace algorithm {


/** (1+(lambda, lambda)) genetic algorithm.

    Reference:

    Benjamin Doerr, Carola Doerr, and Franziska Ebel. 2015. From
    black-box complexity to designing new genetic
    algorithms. Theoretical Computer Science 567 (2015), 87–104.

*/
class OnePlusLambdaCommaLambdaGa: public IterativeAlgorithm {

protected:

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

  /** @name Parameters
   */
  ///@{

  /// Mutation rate
  double _mutation_rate;

  /// Crossover bias
  double _crossover_bias;

  ///@}

  /** @name Loop
   */
  ///@{

  /// Initialize
  void init() override;

  /// Single iteration
  void iterate() override;

  ///@}

public:

  /** Constructor.

      By default, _mutation_rate is set to lambda / n and
      _crossover_bias to 1 / lambda.

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

    _mutation_rate = double(lambda) / double(n);
    _crossover_bias = 1 / double(lambda);
  }

  /** @name Setters
   */
  ///@{

  /// Set the mutation rate
  void set_mutation_rate(double p) { _mutation_rate = p; }

  /// Set the crossover bias
  void set_crossover_bias(double x) { _crossover_bias = x; }

  ///@}

};


} // end of namespace algorithm
} // end of namespace hnco


#endif
