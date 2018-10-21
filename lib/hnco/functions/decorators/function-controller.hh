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

#ifndef HNCO_FUNCTIONS_DECORATORS_FUNCTION_CONTROLLER_H
#define HNCO_FUNCTIONS_DECORATORS_FUNCTION_CONTROLLER_H

#include <assert.h>

#include <iostream>
#include <unordered_map>

#include "hnco/map.hh"

#include "function-decorator.hh"


namespace hnco::function {


  /// Function controller
  class FunctionController:
    public FunctionDecorator {

  public:

    /// Constructor
    FunctionController(Function *function):
      FunctionDecorator(function) {}

    /** @name Information about the function
     */
    ///@{

    /// Get bit vector size
    size_t get_bv_size() { return _function->get_bv_size(); }

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
    double safe_eval(const bit_vector_t& x) { return _function->safe_eval(x); }

    ///@}


    /** @name Display
     */
    ///@{

    /// Display
    void display(std::ostream& stream) { _function->display(stream); }

    /// Describe a bit vector
    void describe(const bit_vector_t& x, std::ostream& stream) { _function->describe(x, stream); }

    ///@}

  };


  /** Stop on maximum.

      The eval() member function throws a MaximumReached exception
      when its argument maximizes the decorated function.
  */
  class StopOnMaximum:
    public FunctionController {

  public:

    /** Constructor.
        \param function Decorated function
        \pre function->has_known_maximum() */
    StopOnMaximum(Function *function):
      FunctionController(function)
    {
      assert(function->has_known_maximum());
    }

    /** @name Evaluation
     */
    ///@{

    /** Evaluate a bit vector.
        \throw MaximumReached */
    double eval(const bit_vector_t&);

    /** Incremental evaluation.
        \throw MaximumReached */
    double incremental_eval(const bit_vector_t& x, double value, const hnco::sparse_bit_vector_t& flipped_bits);

    /** Update after a safe evaluation.
        \throw MaximumReached */
    void update(const bit_vector_t& x, double value);

    ///@}

  };


  /// Stop on target
  class StopOnTarget:
    public FunctionController {

    /// Target
    double _target;

  public:

    /** Constructor.
        \param function Decorated function */
    StopOnTarget(Function *function, double target):
      FunctionController(function),
      _target(target) {}

    /** @name Evaluation
     */
    ///@{

    /** Evaluate a bit vector.
        \throw TargetReached */
    double eval(const bit_vector_t&);

    /** Incremental evaluation.
        \throw TargetReached */
    double incremental_eval(const bit_vector_t& x, double value, const hnco::sparse_bit_vector_t& flipped_bits);

    /** Update after a safe evaluation.
        \throw TargetReached */
    void update(const bit_vector_t& x, double value);

    ///@}

  };


  /// Call counter
  class CallCounter:
    public FunctionController {

  protected:

    /// Number of calls
    int _num_calls;

  public:

    /// Constructor
    CallCounter(Function *function):
      FunctionController(function),
      _num_calls(0) {}

    /** @name Evaluation
     */
    ///@{

    /// Evaluate a bit vector
    double eval(const bit_vector_t&);

    /// Incremental evaluation
    double incremental_eval(const bit_vector_t& x, double value, const hnco::sparse_bit_vector_t& flipped_bits);

    /// Update after a safe evaluation
    void update(const bit_vector_t& x, double value);

    ///@}


    /// Get the number of calls
    int get_num_calls() { return _num_calls; }

  };


  /** ProgressTracker.

      A ProgressTracker is a CallCounter which records the last event,
      that is the time and value of the last improvement.
  */    
  class ProgressTracker:
    public CallCounter {

  public:

    /// Event
    struct Event {

      /// Time
      int time;

      /// Value
      double value;

    };

  protected:

    /// Last improvement
    Event _last_improvement;

    /** @name Parameters
     */
    ///@{

    /// Log improvement
    bool _log_improvement = false;

    /// Output stream
    std::ostream *_stream = &std::cout;

    ///@}

    /// Update last improvement
    void update_last_improvement(double value);

  public:

    /// Constructor
    ProgressTracker(Function *function):
      CallCounter(function)
    {
      _last_improvement.time = 0;
    }

    /** @name Evaluation
     */
    ///@{

    /** Evaluate a bit vector.
        \throw MaximumReached
        \throw TargetReached
    */
    double eval(const bit_vector_t&);

    /** Incremental evaluation.
        \throw MaximumReached
        \throw TargetReached
    */
    double incremental_eval(const bit_vector_t& x, double value, const hnco::sparse_bit_vector_t& flipped_bits);

    /** Update after a safe evaluation.
        \throw MaximumReached
        \throw TargetReached
    */
    void update(const bit_vector_t& x, double value);

    ///@}


    /** Get the last improvement.

        \warning If _last_improvement.time is zero then _function has
        never been called. The Event returned by get_last_improvement
        has therefore no meaning.
    */
    const Event& get_last_improvement() { return _last_improvement; }

    /** @name Setters
     */
    ///@{

    /// Log improvement
    void set_log_improvement(bool x) { _log_improvement = x; }

    /// Output stream
    void set_stream(std::ostream *x) { _stream = x; }

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
    double eval(const bit_vector_t&);

    /** Incremental evaluation.
        \throw LastEvaluation */
    double incremental_eval(const bit_vector_t& x, double value, const hnco::sparse_bit_vector_t& flipped_bits);

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
    public FunctionController {

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
      FunctionController(function),
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
    double eval(const bit_vector_t&);

    ///@}

    /// Get lookup ratio
    double get_lookup_ratio() { return double(_num_lookups) / double(_num_evaluations); }

  };


} // end of namespace hnco::function


#endif
