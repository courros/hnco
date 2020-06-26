/* Copyright (C) 2016, 2017, 2018, 2019, 2020 Arnaud Berny

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

#include <assert.h>

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
  solution_t _solution;


  /** @name Parameters
   */
  ///@{

  /// Output stream
  std::ostream *_stream = &std::cout;

  /// Log context
  LogContext *_log_context = nullptr;

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

  /** Update solution (strict).

      \warning Evaluates the function once.
  */
  void update_solution(const bit_vector_t& x);

  /// Update solution (strict)
  void update_solution(const solution_t& pv);

  ///@}

  /// Set functions
  void set_functions(const std::vector<function::Function *>& functions) {
    assert(!functions.empty());
    assert(functions[0]);

    _functions = functions;
    _function = functions[0];
  }

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

  /// Maximize
  virtual void maximize(const std::vector<function::Function *>& functions) = 0;

  /** Finalize.

      Does nothing.

      It is usually overridden by algorithms which do not keep
      _solution up-to-date. In case _function throws a
      LastEvaluation exception, the algorithm might leave _solution
      in an undefined state. This can be fixed in this member
      function.
  */
  virtual void finalize() {}

  ///@}


  /** @name Getters
   */
  ///@{

  /// Get bit vector size
  int get_bv_size() { return _solution.first.size(); }

  /// Get the solution
  const solution_t& get_solution() { return _solution; }

  ///@}


  /** @name Setters
   */
  ///@{

  /// Set the output stream
  void set_stream(std::ostream *stream) { _stream = stream; }

  /// Set the log context
  void set_log_context(LogContext *log_context) { _log_context = log_context; }

  ///@}
    
};


} // end of namespace algorithm
} // end of namespace hnco


#endif
