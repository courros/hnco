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

#ifndef HNCO_FUNCTIONS_CONTROLLERS_CONTROLLER_H
#define HNCO_FUNCTIONS_CONTROLLERS_CONTROLLER_H

#include <assert.h>

#include <iostream>
#include <unordered_map>

#include "hnco/stop-watch.hh"
#include "hnco/functions/decorator.hh"


namespace hnco {
namespace function {

/// Controllers
namespace controller {


/// Function controller
class Controller:
    public Decorator {

public:

  /// Constructor
  Controller(Function *function):
    Decorator(function) {}

  /** @name Information about the function
   */
  ///@{

  /// Get bit vector size
  int get_bv_size() { return _function->get_bv_size(); }

  /// Get the global maximum
  double get_maximum() { return _function->get_maximum(); }

  /// Check for a known maximum
  bool has_known_maximum() { return _function->has_known_maximum(); }

  /** Check whether the function provides incremental evaluation.
      \return true if the decorated function does
  */
  bool provides_incremental_evaluation() { return _function->provides_incremental_evaluation(); }

  ///@}


  /** @name Evaluation
   */
  ///@{

  /// Safely evaluate a bit vector
  double evaluate_safely(const bit_vector_t& x) { return _function->evaluate_safely(x); }

  ///@}

};


/** Stop on maximum.

    The member function eval throws an exception MaximumReached when
    its argument maximizes the decorated function.

    \warning The maximum is detected using the equality operator
    hence the result should be taken with care in case of non
    integer (floating point) function values.
*/
class StopOnMaximum:
    public Controller {

public:

  /** Constructor.
      \param function Decorated function
      \pre function->has_known_maximum() */
  StopOnMaximum(Function *function):
    Controller(function)
  {
    assert(function->has_known_maximum());
  }

  /** @name Evaluation
   */
  ///@{

  /** Evaluate a bit vector.
      \throw MaximumReached */
  double evaluate(const bit_vector_t&);

  /** Incrementally evaluate a bit vector.
      \throw MaximumReached */
  double evaluate_incrementally(const bit_vector_t& x, double value, const hnco::sparse_bit_vector_t& flipped_bits);

  /** Update after a safe evaluation.
      \throw MaximumReached */
  void update(const bit_vector_t& x, double value);

  ///@}

};


/** Stop on target.

    The member function eval throws an exception TargetReached when
    the value of its decorated function reaches a given target.

    \warning The target is detected using the greater or equal
    operator hence the result should be taken with care in case of
    non integer (floating point) function values.
*/
class StopOnTarget:
    public Controller {

  /// Target
  double _target;

public:

  /** Constructor.

      \param function Decorated function
      \param target Target
  */
  StopOnTarget(Function *function, double target):
    Controller(function),
    _target(target) {}

  /** @name Evaluation
   */
  ///@{

  /** Evaluate a bit vector.
      \throw TargetReached */
  double evaluate(const bit_vector_t&);

  /** Incrementally evaluate a bit vector.
      \throw TargetReached */
  double evaluate_incrementally(const bit_vector_t& x, double value, const hnco::sparse_bit_vector_t& flipped_bits);

  /** Update after a safe evaluation.
      \throw TargetReached */
  void update(const bit_vector_t& x, double value);

  ///@}

};


/// Call counter
class CallCounter:
    public Controller {

protected:

  /// Number of calls
  int _num_calls;

public:

  /// Constructor
  CallCounter(Function *function):
    Controller(function),
    _num_calls(0) {}

  /** @name Evaluation
   */
  ///@{

  /// Evaluate a bit vector
  double evaluate(const bit_vector_t&);

  /// Incrementally evaluate a bit vector
  double evaluate_incrementally(const bit_vector_t& x, double value, const hnco::sparse_bit_vector_t& flipped_bits);

  /// Update after a safe evaluation
  void update(const bit_vector_t& x, double value);

  ///@}


  /// Get the number of calls
  int get_num_calls() { return _num_calls; }

};


/** ProgressTracker.

    A ProgressTracker is a CallCounter which keeps track the last
    improvement, that is its value and the number of evaluations
    needed to reach it.
*/    
class ProgressTracker:
    public CallCounter {

public:

  /// %Event
  struct Event {

    /// Number of evaluations
    int num_evaluations;

    /// Value
    double value;

  };

protected:

  /// Last improvement
  Event _last_improvement;

  /// Stop watch
  StopWatch _stop_watch;

  /** @name Parameters
   */
  ///@{

  /// Log improvement
  bool _log_improvement = false;

  /// Record evaluation time
  bool _record_evaluation_time = false;

  ///@}

  /// Update last improvement
  void update_last_improvement(double value);

public:

  /// Constructor
  ProgressTracker(Function *function):
    CallCounter(function)
  {
    _last_improvement.num_evaluations = 0;
  }

  /** @name Evaluation
   */
  ///@{

  /// Evaluate a bit vector
  double evaluate(const bit_vector_t&);

  /// Incrementally evaluate a bit vector
  double evaluate_incrementally(const bit_vector_t& x, double value, const hnco::sparse_bit_vector_t& flipped_bits);

  /// Update after a safe evaluation
  void update(const bit_vector_t& x, double value);

  ///@}


  /** @name Get information
   */
  ///@{

  /** Get the last improvement.

      \warning If _last_improvement.num_evaluations is zero then
      _function has never been called. The Event returned by
      get_last_improvement has therefore no meaning.
  */
  const Event& get_last_improvement() { return _last_improvement; }

  /// Get evaluation time
  double get_evaluation_time() { return _stop_watch.get_total_time(); }

  ///@}

  /** @name Setters
   */
  ///@{

  /// Log improvement
  void set_log_improvement(bool x) { _log_improvement = x; }

  /// Record evaluation time
  void set_record_evaluation_time(bool b) { _record_evaluation_time = b; }

  ///@}

};


/// Insert formatted output
std::ostream& operator<<(std::ostream& stream, const ProgressTracker::Event& event);


/// CallCounter with a limited number of evaluations
class OnBudgetFunction:
    public CallCounter {

  /// Budget
  int _budget;

public:

  /// Constructor
  OnBudgetFunction(Function *function, int budget):
    CallCounter(function),
    _budget(budget) {}

  /** @name Evaluation
   */
  ///@{

  /** Evaluate a bit vector.
      \throw LastEvaluation */
  double evaluate(const bit_vector_t&);

  /** Incrementally evaluate a bit vector.
      \throw LastEvaluation */
  double evaluate_incrementally(const bit_vector_t& x, double value, const hnco::sparse_bit_vector_t& flipped_bits);

  /** Update after a safe evaluation
      \throw LastEvaluation */
  void update(const bit_vector_t& x, double value);

  ///@}

};


/** Cache.

    This is a naive approach, in particular with respect to time
    complexity. Moreover, there is no control on the size of the
    database.

    There is no default hash function for std::vector<char> hence
    the need to first copy a bit_vector_t into a std::vector<bool>,
    for which such a function exists, before inserting it or
    checking its existence in the map.
*/
class Cache:
    public Controller {

  /// Cache
  std::unordered_map<std::vector<bool>, double> _cache;

  /// Key
  std::vector<bool> _key;

  /// Evaluation counter
  int _num_evaluations;

  /// Lookup counter
  int _num_lookups;

public:

  /** Constructor.
      \param function Decorated function */
  Cache(Function *function):
    Controller(function),
    _key(function->get_bv_size()),
    _num_evaluations(0),
    _num_lookups(0) {}

  /** Check whether the function provides incremental evaluation.
      \return false
  */
  bool provides_incremental_evaluation() { return false; }

  /** @name Evaluation
   */
  ///@{

  /// Evaluate a bit vector
  double evaluate(const bit_vector_t&);

  ///@}

  /// Get lookup ratio
  double get_lookup_ratio() { return double(_num_lookups) / double(_num_evaluations); }

};


} // end of namespace controller
} // end of namespace function
} // end of namespace hnco


#endif
