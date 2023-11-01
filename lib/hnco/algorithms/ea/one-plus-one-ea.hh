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

#ifndef HNCO_ALGORITHMS_EA_ONE_PLUS_ONE_EA_H
#define HNCO_ALGORITHMS_EA_ONE_PLUS_ONE_EA_H

#include "hnco/algorithms/algorithm.hh"
#include "hnco/algorithms/local-search/random-local-search.hh"
#include "hnco/neighborhoods/neighborhood.hh"


namespace hnco {
namespace algorithm {


/** (1+1) EA.

    (1+1) EA is implemented as a RandomLocalSearch with a
    StandardBitMutation neighborhood and infinite patience. Thus the
    class OnePlusOneEa is derived from Algorithm instead of
    IterativeAlgorithm.

    Reference:

    Thomas Jansen, Analyzing Evolutionary Algorithms. Springer, 2013.

*/
class OnePlusOneEa:
    public Algorithm {

  /// Neighborhood
  neighborhood::StandardBitMutation _neighborhood;

  /// Random local search
  RandomLocalSearch _rls;

  /** @name Parameters
   */
  ///@{

  /// Number of iterations
  int _num_iterations = 0;

  /// Mutation rate
  double _mutation_rate;

  /// Allow no mutation
  bool _allow_no_mutation = false;

  /// Incremental evaluation
  bool _incremental_evaluation = false;

  ///@}

public:

  /** Constructor.

      \param n Size of bit vectors

      _mutation_rate is initialized to 1 / n.
  */
  OnePlusOneEa(int n):
    Algorithm(n),
    _neighborhood(n),
    _rls(n, &_neighborhood),
    _mutation_rate(1 / double(n)) {}

  /// Maximize
  void maximize(const std::vector<function::Function *>& functions) override {
    _neighborhood.set_mutation_rate(_mutation_rate);
    _neighborhood.set_allow_no_mutation(_allow_no_mutation);
    _rls.set_num_iterations(_num_iterations);
    _rls.set_incremental_evaluation(_incremental_evaluation);
    _rls.set_patience(0);
    _rls.maximize(functions);
  }

  /// Finalize
  void finalize() override {
    _rls.finalize();
    _solution = _rls.get_solution();
  }

  /** @name Setters
   */
  ///@{

  /** Set the number of iterations.

      \param x Number of iterations

      x <= 0 means indefinite */
  void set_num_iterations(int x) { _num_iterations = x; }

  /// Set the mutation rate
  void set_mutation_rate(double p) { _mutation_rate = p; }

  /// Set the flag _allow_no_mutation
  void set_allow_no_mutation(bool b) { _allow_no_mutation = b; }

  /// Set incremental evaluation
  void set_incremental_evaluation(bool x) { _incremental_evaluation = x; }

  ///@}

};


} // end of namespace algorithm
} // end of namespace hnco


#endif
