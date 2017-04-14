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

#ifndef HNCO_FUNCTIONS_FUNCTION_DECORATOR_H
#define HNCO_FUNCTIONS_FUNCTION_DECORATOR_H

#include <assert.h>

#include <unordered_map>

#include "hnco/map.hh"

#include "function.hh"


namespace hnco {
namespace function {


  /// Function decorator
  class FunctionDecorator:
    public Function {

  protected:

    /// Decorated function
    Function *_function;

  public:

    /// Constructor
    FunctionDecorator(Function *function):
      _function(function) {}

    /// Constructor
    FunctionDecorator():
      _function(0) {}

    /// Get bit vector size
    size_t get_bv_size() { return _function->get_bv_size(); }

    /// Check for a known maximum.
    bool has_known_maximum() { return _function->has_known_maximum(); }

    /// Get the global maximum
    double get_maximum() { return _function->get_maximum(); }

    /// Display
    void display(std::ostream& stream) { _function->display(stream); }

    /// Describe
    void describe(const bit_vector_t& x, std::ostream& stream) { _function->describe(x, stream); }

  };


  /** Stop on maximum.

      The eval member function throws a MaximumReached exception when
      its argument is the known maximum of the decorated function.

      \warning When a MaximumReached exception is thrown, a
      StopOnMaximum object gives up the control flow to the receiver
      of the exception. In consequence, a search algorithm which does
      not catch the exception cannot keep track of the solution.
  */
  class StopOnMaximum:
    public FunctionDecorator {

  public:

    /** Constructor.
        \param function Decorated function
        \pre function->has_known_maximum() */
    StopOnMaximum(Function *function):
      FunctionDecorator(function)
    {
      assert(function->has_known_maximum());
    }

    /** Evaluate a bit vector.
        \throw MaximumReached */
    double eval(const bit_vector_t&);

    /// Safely evaluate a bit vector
    double safe_eval(const bit_vector_t& x) { return _function->safe_eval(x); }

    /// Update after a safe evaluation
    void update(const bit_vector_t& x, double value);

  };


  /** Negation.

      Use cases:

      - for algorithms which minimize rather than maximize a function
      - for functions one wishes to minimize
      - when minimization is needed inside an algorithm

  */
  class Negation:
    public FunctionDecorator {

  public:

    /// Constructor
    Negation(Function *function):
      FunctionDecorator(function) {}

    /// Constructor
    Negation() {}

    /// Evaluate a bit vector
    double eval(const bit_vector_t&);

    /** Check for a known maximum.
        \return false */
    bool has_known_maximum() { return false; }

    /** Get the global maximum.
        \throw Error */
    double get_maximum() { throw exception::Error("Unknown maximum"); }

  };


  /// Stop on target
  class StopOnTarget:
    public FunctionDecorator {

    /// Target
    double _target;

  public:

    /** Constructor.
        \param function Decorated function */
    StopOnTarget(Function *function, double target):
      FunctionDecorator(function),
      _target(target) {}

    /** Evaluate a bit vector.
        \throw TargetReached */
    double eval(const bit_vector_t&);

    /// Safely evaluate a bit vector
    double safe_eval(const bit_vector_t& x) { return _function->safe_eval(x); }

    /// Update after a safe evaluation
    void update(const bit_vector_t& x, double value);

  };


  /// Composition of a function and a map
  class FunctionMapComposition:
    public FunctionDecorator {

    /// Map
    Map *_map;

    /// _bv
    bit_vector_t _bv;

  public:

    /** Constructor.
        \pre map->get_output_size() == function->get_bv_size()
        \throw Error
    */
    FunctionMapComposition(Function *function, Map *map):
      FunctionDecorator(function),
      _map(map)
    {
      assert(map);

      if (map->get_output_size() != function->get_bv_size())
        throw exception::Error("FunctionMapComposition::FunctionMapComposition: _function and _map must be compatible");
      _bv.resize(function->get_bv_size());
    }

    /// Get bit vector size
    size_t get_bv_size() { return _map->get_input_size(); }

    /// Evaluate a bit vector
    double eval(const bit_vector_t&);

    /** Check for a known maximum.
        \return true if the function has a known maximum and the map
        is bijective. */
    bool has_known_maximum() {
      return
        _function->has_known_maximum() &&
        _map->is_surjective();
    }

    /** Get the global maximum.
        \throw Error */
    double get_maximum() {
      if (has_known_maximum())
        return _function->get_maximum();
      else
        throw exception::Error("Unknown maximum");
    }

  };


  /// Additive Gaussian Noise
  class AdditiveGaussianNoise:
    public FunctionDecorator {

    /// Normal distribution
    std::normal_distribution<double> _dist;

  public:

    /// Constructor
    AdditiveGaussianNoise(Function *function, double stddev):
      FunctionDecorator(function),
      _dist(0, stddev) {}

    /// Evaluate a bit vector
    double eval(const bit_vector_t&);

    /** Check for a known maximum.
        \return false */
    bool has_known_maximum() { return false; }

    /** Get the global maximum.
        \throw Error */
    double get_maximum() { throw exception::Error("Unknown maximum"); }

  };


  /// Call counter
  class CallCounter:
    public FunctionDecorator {

  protected:

    /// Number of calls
    int _num_calls;

  public:

    /// Constructor
    CallCounter(Function *function):
      FunctionDecorator(function),
      _num_calls(0) {}

    /// Evaluate a bit vector
    double eval(const bit_vector_t&);

    /// Safely evaluate a bit vector
    double safe_eval(const bit_vector_t& x) { return _function->safe_eval(x); }

    /// Update after a safe evaluation
    void update(const bit_vector_t& x, double value);

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

    /// Log improvement
    bool _log_improvement;

    /// Update last improvement
    void update_last_improvement(double value);

  public:

    /// Constructor
    ProgressTracker(Function *function, bool log_improvement = false):
      CallCounter(function),
      _log_improvement(log_improvement) {}

    /** Evaluate a bit vector.
        \throw MaximumReached */
    double eval(const bit_vector_t&);

    /// Update after a safe evaluation
    void update(const bit_vector_t& x, double value);

    /// Get the last improvement
    const Event& get_last_improvement() { return _last_improvement; }

  };


  /// Insert formatted output
  std::ostream& operator<<(std::ostream& stream, const ProgressTracker::Event& event);


  /// ProgressTracker with a limited number of evaluations
  class OnBudgetFunction:
    public CallCounter {

    /// Budget
    int _budget;

  public:

    /// Constructor
    OnBudgetFunction(Function *function, int budget):
      CallCounter(function),
      _budget(budget) {}

    /** Evaluate a bit vector.
        \throw LastEvaluation */
    double eval(const bit_vector_t&);

    /// Update after a safe evaluation
    void update(const bit_vector_t& x, double value);

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
    public FunctionDecorator {

    /// Database of past evaluations
    std::unordered_map<std::vector<bool>, double> _cache;

    /// STL bit vector
    std::vector<bool> _x;

    /// Evaluation counter
    int _num_evaluations;

    /// Lookup counter
    int _num_lookups;

  public:

    /** Constructor.
        \param function Decorated function */
    Cache(Function *function):
      FunctionDecorator(function),
      _x(function->get_bv_size()),
      _num_evaluations(0),
      _num_lookups(0) {}

    /// Evaluate a bit vector
    double eval(const bit_vector_t&);

    /// Display
    void display(std::ostream& stream);

  };


} // end of namespace function
} // end of namespace hnco


#endif
