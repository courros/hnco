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

#ifndef HNCO_ALGORITHMS_EA_SELF_ADJUSTING_ONE_PLUS_ONE_EA_H
#define HNCO_ALGORITHMS_EA_SELF_ADJUSTING_ONE_PLUS_ONE_EA_H

#include "hnco/algorithms/iterative-algorithm.hh"
#include "hnco/neighborhoods/neighborhood.hh"


namespace hnco {
namespace algorithm {


/** 
 * Self-adjusting (1+1) EA.
 * Reference:
 * Benjamin Doerr, Carola Doerr, and Johannes Lengler. 2019.
 * Self-adjusting mutation rates with provably optimal success rules.
 * In Proceedings of the Genetic and Evolutionary Computation
 * Conference (GECCO '19). Association for Computing Machinery, New
 * York, NY, USA, 1479–1487. https://doi.org/10.1145/3321707.3321733
 */
class SelfAdjustingOnePlusOneEa: public IterativeAlgorithm {

  /// Mutation operator
  neighborhood::StandardBitMutation _mutation;

  /// Mutation rate
  double _mutation_rate;

  /// Update strength to the power the success rate
  double _coefficient;

  /**
   * @name Parameters
   */
  ///@{

  /// Initial mutation rate
  double _mutation_rate_init;

  /// Minimum mutation rate
  double _mutation_rate_min;

  /// Maximum mutation rate
  double _mutation_rate_max = 1;

  /// Success ratio
  double _success_ratio = 4;

  /// Update strength
  double _update_strength;

  /// Allow no mutation
  bool _allow_no_mutation = false;

  /// Incremental evaluation
  bool _incremental_evaluation = false;

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

  /// Initialize
  void init() override;

  /// Single iteration
  void iterate() override;

  /// Log
  void log() override;

  ///@}

  /// Single iteration with full evaluation
  void iterate_full();

  /// Single iteration with incremental evaluation
  void iterate_incremental();

  /// Set flag for something to log
  void set_something_to_log() { _something_to_log = _log_mutation_rate; }

public:

  /// Constructor
  SelfAdjustingOnePlusOneEa(int n)
    : IterativeAlgorithm(n)
    , _mutation(n)
    , _mutation_rate_init(1 / double(n))
    , _mutation_rate_min(1 / double(n * n))
    , _update_strength(1 + 1 / double(n))
  {}

  /// Finalize
  void finalize() override;

  /**
   * @name Setters
   */
  ///@{

  /// Set the initial mutation rate
  void set_mutation_rate_init(double p) { _mutation_rate_init = p; }

  /// Set the minimum mutation rate
  void set_mutation_rate_min(double p) { _mutation_rate_min = p; }

  /// Set the maximum mutation rate
  void set_mutation_rate_max(double p) { _mutation_rate_max = p; }

  /// Set update strength
  void set_update_strength(double x) { _update_strength = x; }

  /// Set success ratio
  void set_success_ratio(double x) { _success_ratio = x; }

  /// Allow no mutation
  void set_allow_no_mutation(bool b) { _allow_no_mutation = b; }

  /// Turn on incremental evaluation
  void set_incremental_evaluation(bool b) { _incremental_evaluation = b; }

  ///@}

  /**
   * @name Setters for logging
   */
  ///@{

  /// Log mutation rate
  void set_log_mutation_rate(bool b) { _log_mutation_rate = b; }

  ///@}

};


} // end of namespace algorithm
} // end of namespace hnco


#endif
