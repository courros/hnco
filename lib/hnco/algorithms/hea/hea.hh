/* Copyright (C) 2016, 2017, 2018, 2019, 2020, 2021 Arnaud Berny

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

#ifndef HNCO_ALGORITHMS_HEA_HEA_H
#define HNCO_ALGORITHMS_HEA_HEA_H

#include "hnco/algorithms/iterative-algorithm.hh"
#include "hnco/algorithms/population.hh"
#include "hnco/logging/logger.hh"


namespace hnco {
namespace algorithm {

/// Herding evolutionary algorithm
namespace hea {


/** Herding evolutionary algorithm.

    Reference:

    Arnaud Berny. 2015. Herding Evolutionary %Algorithm. In
    Proceedings of the Companion Publication of the 2015 Annual
    Conference on Genetic and Evolutionary Computation (GECCO
    Companion ’15). ACM, New York, NY, USA, 1355–1356.

*/
template<class Moment, class Herding>
class Hea: public algorithm::IterativeAlgorithm {

  /// Moment
  Moment _target;

  /// Moment of selected individuals
  Moment _selection;

  /// Uniform moment
  Moment _uniform;

  /// Population
  algorithm::Population _population;

  /// Herding
  Herding *_herding;

  /// Error cache
  double _error_cache;

  /// Distance to uniform cache
  double _dtu_cache;

  /// Delta cache
  double _delta_cache;

  /// Selection distance cache
  double _selection_cache;

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

  /// Log error
  bool _log_error = false;

  /// Log distance to uniform
  bool _log_dtu = false;

  /// Log delta (moment increment)
  bool _log_delta = false;

  /// Log the moment matrix
  bool _log_moment = false;

  ///@}

  /** @name Loop
   */
  ///@{

  /// Initialization
  void init() override {
    random_solution();
    _target.uniform();
    _herding->init();
    set_something_to_log();
  }

  /// Single iteration
  void iterate() override {
    if (_reset_period > 0) {
      if (_iteration % _reset_period == 0)
        _herding->init();
    }

    for (int i = 0; i < _population.size(); i++)
      _herding->sample(_target, _population.get_bv(i));

    if (_log_error)
      _error_cache = _herding->error(_target);
    if (_log_dtu)
      _dtu_cache = _target.distance(_uniform);
    if (_log_delta)
      _delta_cache = _herding->get_delta().norm_2();

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
      _log_error ||
      _log_dtu ||
      _log_delta ||
      _log_moment;
  }

  /// Log
  void log() override {
    assert(_something_to_log);

    logging::Logger l(_log_context);

    if (_log_moment) {
      _target.display(logging::Logger::stream());
      return;
    }

    // Single line
    if (_log_error)
      l.line() << _error_cache << " ";

    if (_log_dtu)
      l.line() << _dtu_cache << " ";

    if (_log_delta)
      l.line() << _delta_cache << " ";

  }

  ///@}

public:

  /** Constructor.

      \param n Size of bit vectors
      \param population_size Population size

      _margin is initialized to 1 / n.
  */
  Hea(int n, int population_size):
    IterativeAlgorithm(n),
    _target(n),
    _selection(n),
    _uniform(n),
    _population(population_size, n),
    _margin(1 / double(n))
  {
    _uniform.uniform();
  }

  /** @name Setters
   */
  ///@{

  /// Set the herding algorithm
  void set_herding(Herding *x) {
    assert(x);
    _herding = x;
  }

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

  ///@}

  /** @name Setters for logging
   */
  ///@{

  /// Log error
  void set_log_error(bool b) { _log_error = b; }

  /// Log distance to uniform
  void set_log_dtu(bool b) { _log_dtu = b; }

  /// Log delta (moment increment)
  void set_log_delta(bool b) { _log_delta = b; }

  /// Log the moment matrix
  void set_log_moment(bool b) { _log_moment = b; }

  ///@}

};


} // end of namespace hea
} // end of namespace algorithm
} // end of namespace hnco


#endif
