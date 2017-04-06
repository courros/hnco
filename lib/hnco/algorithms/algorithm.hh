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

#ifndef HNCO_ALGORITHMS_ALGORITHM_H
#define HNCO_ALGORITHMS_ALGORITHM_H

#include "hnco/functions/function.hh"


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
    bit_vector_t _solution;

    /// Maximum
    double _maximum;

    /// Random initialization of solution
    virtual void random_solution();

    /// Set solution and maximum
    virtual void set_solution(const bit_vector_t& x);

    /// Set solution and maximum
    virtual void set_solution(const bit_vector_t& x, double value);

    /// Update solution and maximum
    virtual void update_solution(const bit_vector_t& x);

    /// Update solution and maximum
    virtual void update_solution(const bit_vector_t& x, double value);

  public:

    /// Constructor
    Algorithm(int n);

    /// Destructor
    virtual ~Algorithm() {}

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

    /// Initialization
    virtual void init() {
      assert(_function);
      random_solution();
    }

    /// Maximize
    virtual void maximize() = 0;

    /// Solution
    virtual const bit_vector_t& get_solution() { return _solution; }

    /// Maximum
    virtual double get_maximum() { return _maximum; }

  };


  /// Iterative search
  class IterativeAlgorithm:
    public Algorithm {

  protected:

    /// Something to log
    bool _something_to_log;

    /// Current iteration
    int _iteration;


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

    /** Number of iterations.
        _num_iterations <= 0 means indefinite */
    int _num_iterations = 0;

  };


} // end of namespace algorithm
} // end of namespace hnco


#endif
