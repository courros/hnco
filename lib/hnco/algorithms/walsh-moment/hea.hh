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

#ifndef HNCO_ALGORITHMS_WALSH_MOMENT_HEA_H
#define HNCO_ALGORITHMS_WALSH_MOMENT_HEA_H

#include "hnco/algorithms/iterative-algorithm.hh"
#include "hnco/algorithms/population.hh"
#include "hnco/logging/logger.hh"


namespace hnco {
namespace algorithm {

/// Algorithms using Walsh moments
namespace walsh_moment {


/** Herding evolutionary algorithm.

    Reference:

    Arnaud Berny. 2015. Herding Evolutionary %Algorithm. In
    Proceedings of the Companion Publication of the 2015 Annual
    Conference on Genetic and Evolutionary Computation (GECCO
    Companion ’15). ACM, New York, NY, USA, 1355–1356.

*/
template<class Herding>
class Hea: public algorithm::IterativeAlgorithm {

  /// Target moment
  typename Herding::Moment _target;

  /// Moment of selected individuals
  typename Herding::Moment _selection;

  /// %Population
  algorithm::Population _population;

  /// Herding
  Herding _herding;

  /// Herding error (moment discrepancy)
  double _herding_error;

  /// Target 2-norm (distance to uniform moment)
  double _target_norm;

  /// Delta (moment increment) 2-norm
  double _delta_norm;

  /** @name Parameters
   */
  ///@{

  /// Moment margin
  double _margin;

  /// Selection size
  int _selection_size = 1;

  /// Reset period
  int _reset_period = 0;

  /// Learning rate
  double _learning_rate = 1e-4;

  /// Bound moment after update
  bool _bound_moment = false;

  ///@}

  /** @name Logging
   */
  ///@{

  /// Log herding error (moment discrepancy)
  bool _log_herding_error = false;

  /// Log target 2-norm (distance to uniform moment)
  bool _log_target_norm = false;

  /// Log delta 2-norm (moment increment)
  bool _log_delta_norm = false;

  /// Log target moment as a symmetric matrix
  bool _log_target = false;

  ///@}

  /** @name Loop
   */
  ///@{

  /// Initialization
  void init() override {
    random_solution();
    _target.init();
    _herding.init();
    set_something_to_log();
  }

  /// Single iteration
  void iterate() override {
    if (_reset_period > 0) {
      if (_iteration % _reset_period == 0)
        _herding.init();
    }

    for (int i = 0; i < _population.get_size(); i++)
      _herding.sample(_target, _population.bvs[i]);

    if (_log_herding_error)
      _herding_error = _herding.error(_target);
    if (_log_target_norm)
      _target_norm = _target.norm_2();
    if (_log_delta_norm)
      _delta_norm = _herding.get_delta().norm_2();

    if (_functions.size() > 1)
      _population.evaluate_in_parallel(_functions);
    else
      _population.evaluate(_function);
    _population.sort();
    update_solution(_population.get_best_bv(),
                    _population.get_best_value());

    _selection.init();
    for (int i = 0; i < _selection_size; i++)
      _selection.add(_population.get_best_bv(i));
    if (_selection_size > 1)
      _selection.average(_selection_size);

    _target.update(_selection, _learning_rate);
    if (_bound_moment)
      _target.bound(_margin);
  }

  /// Set flag for something to log
  void set_something_to_log() {
    _something_to_log =
      _log_herding_error ||
      _log_target_norm ||
      _log_delta_norm ||
      _log_target;
  }

  /// Log
  void log() override {
    assert(_something_to_log);

    if (_log_target) {
      _target.display(logging::Logger::stream());
      return;
    }

    logging::Logger l(_log_context);

    // Single line
    if (_log_herding_error)
      l.line() << _herding_error << " ";

    if (_log_target_norm)
      l.line() << _target_norm << " ";

    if (_log_delta_norm)
      l.line() << _delta_norm << " ";

  }

  ///@}

public:

  /** Constructor.

      \param n Size of bit vectors
      \param population_size Population size

      _margin is initialized to 1 / n.
  */
  Hea(int n, int population_size)
    : IterativeAlgorithm(n)
    , _target(n)
    , _selection(n)
    , _population(population_size, n)
    , _herding(n)
    , _margin(1 / double(n))
  {}

  /** @name Setters
   */
  ///@{

  /// Set the moment margin
  void set_margin(double x) { _margin = x; }

  /** Set the selection size.

      The selection size is the number of selected individuals in
      the population.
  */
  void set_selection_size(int x) { _selection_size = x; }

  /** Set the reset period

      \param x Reset period

      x <= 0 means no reset.
  */
  void set_reset_period(int x) { _reset_period = x; }

  /// Set the learning rate
  void set_learning_rate(double x) { _learning_rate = x; }

  /// Set the bound moment after update
  void set_bound_moment(bool x) { _bound_moment = x; }

  /// Randomize bit order
  void set_randomize_bit_order(bool b) { _herding.set_randomize_bit_order(b); }

  ///@}

  /** @name Setters for logging
   */
  ///@{

  /// Log herding error (moment discrepancy)
  void set_log_herding_error(bool b) { _log_herding_error = b; }

  /// Log target 2-norm (distance to uniform moment)
  void set_log_target_norm(bool b) { _log_target_norm = b; }

  /// Log delta (moment increment) 2-norm
  void set_log_delta_norm(bool b) { _log_delta_norm = b; }

  /// Log target moment as a symmetric matrix
  void set_log_target(bool b) { _log_target = b; }

  ///@}

};


} // end of namespace walsh_moment
} // end of namespace algorithm
} // end of namespace hnco


#endif
