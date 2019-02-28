/* Copyright (C) 2016, 2017, 2018, 2019 Arnaud Berny

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

#ifndef HNCO_ALGORITHMS_ALGORITHM_H
#define HNCO_ALGORITHMS_ALGORITHM_H

#include <iostream>

#include "hnco/exception.hh"
#include "hnco/functions/function.hh"

#include "log-context.hh"


namespace hnco {
/// Algorithms
namespace algorithm {

  /** Abstract search algorithm.

      All algorithms maximize some given function, sometimes called a
      fitness function or an objective function.
  */
  class Algorithm {

  protected:

    /// Function
    function::Function *_function;

    /** Functions.

        Each thread has its own function.
    */
    std::vector<function::Function *> _functions;

    /// Solution
    point_value_t _solution;

    /// Log context
    LogContext *_log_context = nullptr;


    /** @name Parameters
     */
    ///@{

    /// Output stream
    std::ostream *_stream = &std::cout;

    ///@}


    /** @name Managing solution
     */
    ///@{

    /// Random solution
    void random_solution();

    /// Set solution
    void set_solution(const bit_vector_t& x, double value);

    /** Set solution.

        \warning Evaluates the function once.
    */
    void set_solution(const bit_vector_t& x);

    /// Update solution (strict)
    void update_solution(const bit_vector_t& x, double value);

    /// Update solution (strict)
    void update_solution(const point_value_t& pv);

    /** Update solution (strict).

        \warning Evaluates the function once.
    */
    void update_solution(const bit_vector_t& x);

    ///@}

  public:

    /// Constructor
    Algorithm(int n):
      _solution(bit_vector_t(n), 0.0)
    {
      if (!(n > 0))
        throw hnco::exception::Error("Algorithm::Algorithm: bit vector size must be positive");
    }

    /// Destructor
    virtual ~Algorithm() {}


    /** @name Optimization
     */
    ///@{

    /// Initialization
    virtual void init() { random_solution(); }

    /// Maximize
    virtual void maximize() = 0;

    ///@}


    /** @name Getters
     */
    ///@{

    /// Solution
    virtual const point_value_t& get_solution() { return _solution; }

    /// Get bit vector size
    virtual size_t get_bv_size() { return _solution.first.size(); }

    ///@}


    /** @name Setters
     */
    ///@{

    /// Set function
    virtual void set_function(function::Function *function) {
      assert(function);
      _function = function;
    }

    /// Set functions
    virtual void set_functions(const std::vector<function::Function *> functions) {
      assert(functions.size() > 0);
      _functions = functions;
    }

    /// Output stream
    void set_stream(std::ostream *x) { _stream = x; }

    /// Set log context
    void set_log_context(LogContext *lc) { _log_context = lc; }

    ///@}
    
  };


  /// Iterative search
  class IterativeAlgorithm:
    public Algorithm {

  protected:

    /// Current iteration
    int _iteration;

    /// Something to log
    bool _something_to_log;

    /** @name Parameters
     */
    ///@{

    /// Number of iterations
    int _num_iterations = 0;

    ///@}

    /// Single iteration
    virtual void iterate() = 0;

    /// Log
    virtual void log() {}

  public:

    /** Constructor.
        \param n Size of bit vectors */
    IterativeAlgorithm(int n):
      Algorithm(n),
      _something_to_log(false) {}

    /** Maximize.

        Inside the loop:
        - call iterate()
        - call log()

        \warning If an exception such as LocalMaximum is thrown by
        iterate(), log() will not be called. However, hnco reports the
        maximum at the end of the search. */
    void maximize();

    /** @name Setters
     */
    ///@{

    /** Set the number of iterations.

        \param x Number of iterations

        x <= 0 means indefinite */
    void set_num_iterations(int x) { _num_iterations = x; }

    ///@}

  };


} // end of namespace algorithm
} // end of namespace hnco


#endif
