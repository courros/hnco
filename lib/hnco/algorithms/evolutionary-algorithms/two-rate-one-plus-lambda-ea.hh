/* Copyright (C) 2016, 2017, 2018, 2019, 2020, 2021, 2022, 2023, 2024 Arnaud Berny

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

#include "hnco/algorithms/iterative-algorithm.hh"
#include "hnco/algorithms/population.hh"
#include "hnco/neighborhoods/neighborhood.hh"

namespace hnco {
namespace algorithm {

/**
 * Two-rate (1+lambda) evolutionary algorithm.
 *
 * Reference:
 *
 * Benjamin Doerr, Christian Gießen, Carsten Witt, and Jing Yang.
 * 2017. The (1+lambda) evolutionary algorithm with self-adjusting
 * mutation rate. In Proceedings of the Genetic and Evolutionary
 * Computation Conference (GECCO '17). Association for Computing
 * Machinery, New York, NY, USA, 1351–1358.
 * https://doi.org/10.1145/3071178.3071279
 */
class TwoRateOnePlusLambdaEa: public IterativeAlgorithm {
public:
  /// Constructor
  TwoRateOnePlusLambdaEa(int n, int population_size)
    : IterativeAlgorithm(n)
    , _population(population_size, n)
    , _mutation_operator(n)
    , _mutation_rate_init(2 / double(n))
    , _mutation_rate_min(2 / double(n))
  {}

  /**
   * @name Setters
   */
  ///@{
  /// Set the initial mutation rate
  void set_mutation_rate_init(double rate) { _mutation_rate_init = rate; }
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
  /// Minimum mutation rate
  double _mutation_rate_min;
  /// Maximum mutation rate
  double _mutation_rate_max = 0.25;
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
  void iterate() override;
  /// Log
  void log() override;
  ///@}

  /// Set flag for something to log
  void set_something_to_log() { _something_to_log = _log_mutation_rate; }
};

} // end of namespace algorithm
} // end of namespace hnco

#endif
