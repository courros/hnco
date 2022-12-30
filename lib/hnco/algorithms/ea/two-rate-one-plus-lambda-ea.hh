/* Copyright (C) 2016, 2017, 2018, 2019, 2020, 2021, 2022 Arnaud Berny

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

#ifndef HNCO_ALGORITHMS_EA_TWO_RATE_ONE_PLUS_LAMBDA_EA
#define HNCO_ALGORITHMS_EA_TWO_RATE_ONE_PLUS_LAMBDA_EA

#include <assert.h>

#include <hnco/algorithms/iterative-algorithm.hh>
#include <hnco/algorithms/population.hh>
#include <hnco/neighborhoods/neighborhood.hh>


namespace hnco {
namespace algorithm {


/**
 * Two-rate (1+lambda) evolutionary algorithm.
 */
class TwoRateOnePlusLambdaEa: public IterativeAlgorithm {

public:

  /// Constructor
  TwoRateOnePlusLambdaEa(int n, int population_size)
    : IterativeAlgorithm(n)
    , _population(population_size, n)
    , _mutation_operator(n)
  {}

  /**
   * @name Setters
   */
  ///@{

  /// Set the initial mutation rate
  void set_mutation_rate_init(double r) { _mutation_rate_init = r; }

  /// Allow no mutation
  void set_allow_no_mutation(bool b) { _allow_no_mutation = b; }

  ///@}

  /**
   * @name Setters for logging
   */
  ///@{

  /// Log mutation rate
  void set_log_mutation_rate(bool b) { _log_mutation_rate = b; }

  ///@}

protected:

  /// Population
  Population _population;

  /// Mutation operator
  neighborhood::StandardBitMutation _mutation_operator;

  /// Mutation rate
  double _mutation_rate;

  /**
   * @name Parameters
   */
  ///@{

  /// Initial mutation rate
  double _mutation_rate_init;

  /// Allow no mutation
  bool _allow_no_mutation = false;

  ///@}

  /**
   * @name Logging
   */
  ///@{

  /// Log entropy
  bool _log_mutation_rate = false;

  ///@}

  /**
   * @name Loop
   */
  ///@{

  /// Initialization
  void init() override;

  /// Single iteration
  void iterate();

  /// Log
  void log() override;

  ///@}

  /// Set flag for something to log
  void set_something_to_log() { _something_to_log = _log_mutation_rate; }

};


} // end of namespace algorithm
} // end of namespace hnco


#endif
