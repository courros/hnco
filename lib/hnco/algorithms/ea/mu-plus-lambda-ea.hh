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

#ifndef HNCO_ALGORITHMS_EA_MU_PLUS_LAMBDA_H
#define HNCO_ALGORITHMS_EA_MU_PLUS_LAMBDA_H

#include "hnco/algorithms/iterative-algorithm.hh"
#include "hnco/algorithms/population.hh"
#include "hnco/neighborhoods/neighborhood.hh"
#include "hnco/random.hh"


namespace hnco {
namespace algorithm {

/** (mu+lambda) EA.

    Reference:

    Thomas Jansen, Analyzing Evolutionary Algorithms. Springer, 2013.

*/
class MuPlusLambdaEa:
    public IterativeAlgorithm {

protected:

  /// Parents
  Population _parents;

  /// Offsprings
  Population _offsprings;

  /// Mutation operator
  neighborhood::StandardBitMutation _mutation;

  /// Select parent
  std::uniform_int_distribution<int> _select_parent;

  /** @name Parameters
   */
  ///@{

  /// Mutation probability
  double _mutation_probability;

  /// Allow no mutation
  bool _allow_no_mutation = false;

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

      \param n Size of bit vectors
      \param mu Parent population size
      \param lambda Offspring population size
  */
  MuPlusLambdaEa(int n, int mu, int lambda):
    IterativeAlgorithm(n),
    _parents(mu, n),
    _offsprings(lambda, n),
    _mutation(n),
    _select_parent(0, mu - 1),
    _mutation_probability(1 / double(n)) {};

  /** @name Setters
   */
  ///@{

  /// Set the mutation probability
  void set_mutation_probability(double x) { _mutation_probability = x; }

  /// Set the flag _allow_no_mutation
  void set_allow_no_mutation(bool b) { _allow_no_mutation = b; }

  ///@}

};


} // end of namespace algorithm
} // end of namespace hnco


#endif
