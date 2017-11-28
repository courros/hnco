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

#ifndef HNCO_ALGORITHMS_HEA_H
#define HNCO_ALGORITHMS_HEA_H

#include <math.h>               // exp, sqrt

#include <bitset>

#include "hnco/algorithms/algorithm.hh"
#include "hnco/algorithms/matrix.hh"
#include "hnco/algorithms/population.hh"
#include "hnco/exception.hh"


/// Herding evolutionary algorithm
namespace hnco::algorithm::hea {

  /// Herding evolutionary algorithm
  template<class Moment, class Herding>
  class Hea:
    public algorithm::IterativeAlgorithm {

  public:

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

    /// Herding
    Herding _herding;

  private:

    /// Moment
    Moment _target;

    /// Moment of selected individuals
    Moment _selection;

    /// Uniform moment
    Moment _uniform;

    /// Population
    algorithm::Population _population;

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


    /// Single iteration
    void iterate() {
      if (_reset_period > 0) {
        if (_iteration % _reset_period == 0)
          _herding.init();
      }

      for (size_t i = 0; i < _population.size(); i++)
        _herding.sample(_target, _population.get_bv(i));

      if (_log_flags[LOG_ERROR])
        _error_cache = _herding.error(_target);

      if (_log_flags[LOG_DTU])
        _dtu_cache = _target.distance(_uniform);

      if (_log_flags[LOG_DELTA])
        _delta_cache = _herding.delta(_target);

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
        _ostream << _error_cache << " ";

      if (_log_flags[LOG_DTU])
        _ostream << _dtu_cache << " ";

      if (_log_flags[LOG_DELTA])
        _ostream << _delta_cache << " ";

      if (_log_flags[LOG_SELECTION])
        _ostream << _selection_cache << " ";

      _ostream << std::endl;
    }

  public:

    /// Constructor
    Hea(int n, int population_size):
      IterativeAlgorithm(n),
      _herding(n),
      _target(n),
      _selection(n),
      _uniform(n),
      _population(population_size, n),
      _margin(1 / double(n))
    {
      _uniform.uniform();
    }

    enum {
      /// Constant rate
      RATE_CONSTANT,

      /// Exponentiel decay
      RATE_EXPONENTIAL,

      /// Inverse decay
      RATE_INVERSE
    };

    /** @name Parameters
     */
    ///@{

    /// Moment margin
    double _margin;

    /// Selection size (number of selected individuals in the population)
    int _selection_size = 1;

    /// Rate strategy
    int _rate_strategy = RATE_CONSTANT;

    /// Reset period (<= 0 means no reset)
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

    /// Initialization
    void init() {
        random_solution();
        _target.uniform();
        _herding.init();
    }

    /// Set log flags
    void set_log_flags(const log_flags_t& lf) {
      _log_flags = lf;
      _something_to_log = _log_flags.any();
    }

    /// Set weight
    void set_weight(double weight) {
      _target._weight = weight;
      _selection._weight = weight;
      _uniform._weight = weight;
    }

  };

} // end of namespace hnco::algorithm::hea


#endif
