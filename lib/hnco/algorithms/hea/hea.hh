/* Copyright (C) 2016, 2017, 2018 Arnaud Berny

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

#include <math.h>               // exp, sqrt

#include <bitset>

#include "hnco/algorithms/algorithm.hh"
#include "hnco/algorithms/matrix.hh"
#include "hnco/algorithms/population.hh"
#include "hnco/exception.hh"


/// Herding evolutionary algorithm
namespace hnco::algorithm::hea {


  /** Herding evolutionary algorithm.

      Reference:

      Arnaud Berny. 2015. Herding Evolutionary %Algorithm. In
      Proceedings of the Companion Publication of the 2015 Annual
      Conference on Genetic and Evolutionary Computation (GECCO
      Companion ’15). ACM, New York, NY, USA, 1355–1356.

  */
  template<class Moment, class Herding>
  class Hea:
    public algorithm::IterativeAlgorithm {

  public:

    enum {
      /// Constant rate
      RATE_CONSTANT,

      /// Exponentiel decay
      RATE_EXPONENTIAL,

      /// Inverse decay
      RATE_INVERSE
    };

    enum {
      /// Log error
      LOG_ERROR,

      /// Log distance to uniform
      LOG_DTU,

      /// Log delta (moment increment)
      LOG_DELTA,

      /// Log the distance between the target and the selection moment
      LOG_SELECTION,

      LAST_LOG
    };

    /// Type for log flags
    typedef std::bitset<LAST_LOG> log_flags_t;

  private:

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

    /** @name Logging
     */
    ///@{

    /// Error cache
    double _error_cache;

    /// Distance to uniform cache
    double _dtu_cache;

    /// Delta cache
    double _delta_cache;

    /// Selection distance cache
    double _selection_cache;

    /// Log flags
    log_flags_t _log_flags;

    ///@}

    /** @name Parameters
     */
    ///@{

    /// Moment margin
    double _margin;

    /// Selection size
    int _selection_size = 1;

    /// Rate strategy
    int _rate_strategy = RATE_CONSTANT;

    /// Reset period
    int _reset_period = 0;

    /// Delay
    int _delay = 10000;

    /// Initial value of the learning rate
    double _initial_rate = 1e-4;

    /// Time constant
    double _time_constant = 1000;

    /// Bound moment after update
    bool _bound_moment = false;

    ///@}

    /// Single iteration
    void iterate() {
      if (_reset_period > 0) {
        if (_iteration % _reset_period == 0)
          _herding->init();
      }

      for (size_t i = 0; i < _population.size(); i++)
        _herding->sample(_target, _population.get_bv(i));

      if (_log_flags[LOG_ERROR])
        _error_cache = _herding->error(_target);

      if (_log_flags[LOG_DTU])
        _dtu_cache = _target.distance(_uniform);

      if (_log_flags[LOG_DELTA])
        _delta_cache = _herding->get_delta().norm_2();

      if (_functions.size() > 1)
        _population.eval(_functions);
      else
        _population.eval(_function);

      _population.sort();

      update_solution(_population.get_best_bv(),
                      _population.get_best_value());

      double rate = _initial_rate;

      if (_iteration > _delay)
        switch (_rate_strategy) {
        case RATE_CONSTANT:
          break;
        case RATE_EXPONENTIAL:
          rate = _initial_rate * exp(-double(_iteration - _delay) / double(_time_constant));
          break;
        case RATE_INVERSE:
          rate = _initial_rate / double(_iteration - _delay);
          break;
        default:
          std::ostringstream stream;
          stream << _rate_strategy;
          throw exception::Error("Hea::iterate: Unknown rate strategy: " + stream.str());
        }

      _selection.init();
      for (int i = 0; i < _selection_size; i++)
        _selection.add(_population.get_best_bv(i));
      if (_selection_size > 1)
        _selection.average(_selection_size);

      if (_log_flags[LOG_SELECTION])
        _selection_cache = _target.distance(_selection);

      _target.update(_selection, rate);

      if (_bound_moment)
        _target.bound(_margin);
    }

    /// Log
    void log() {
      assert(_log_flags.any());

      if (_log_flags[LOG_ERROR])
        (*_stream) << _error_cache << " ";

      if (_log_flags[LOG_DTU])
        (*_stream) << _dtu_cache << " ";

      if (_log_flags[LOG_DELTA])
        (*_stream) << _delta_cache << " ";

      if (_log_flags[LOG_SELECTION])
        (*_stream) << _selection_cache << " ";

      (*_stream) << std::endl;
    }

  public:

    /** Constructor.

        \param n Size of bit vectors

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

    /// Initialization
    void init() {
        random_solution();
        _target.uniform();
        _herding->init();
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

    /// Set the rate strategy
    void set_rate_strategy(int x) { _rate_strategy = x; }

    /** Set the reset period

        \param x Reset period

        x <= 0 means no reset.
    */
    void set_reset_period(int x) { _reset_period = x; }

    /// Set the delay
    void set_delay(int x) { _delay = x; }

    /// Set the initial value of the learning rate
    void set_initial_rate(double x) { _initial_rate = x; }

    /// Set the time constant
    void set_time_constant(double x) { _time_constant = x; }

    /// Set the bound moment after update
    void set_bound_moment(bool x) { _bound_moment = x; }

    /// Set weight
    void set_weight(double weight) {
      _target._weight = weight;
      _selection._weight = weight;
      _uniform._weight = weight;
    }

    /// Set log flags
    void set_log_flags(const log_flags_t& lf) {
      _log_flags = lf;
      _something_to_log = _log_flags.any();
    }

    ///@}

  };

} // end of namespace hnco::algorithm::hea


#endif
