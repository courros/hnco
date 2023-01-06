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

#ifndef HNCO_MULTIOBJECTIVE_ALGORITHMS_ALGORITHM_H
#define HNCO_MULTIOBJECTIVE_ALGORITHMS_ALGORITHM_H

#include <assert.h>

#include "hnco/logging/log-context.hh"
#include "hnco/multiobjective/functions/function.hh"

#include "population.hh"


namespace hnco {
namespace multiobjective {

/// Multiobjective Algorithms
namespace algorithm {


/** Abstract multiobjective search algorithm.

    All algorithms minimize some given function.
*/
class Algorithm {

public:

  /// Function type
  using Function = hnco::multiobjective::function::Function;

protected:

  /** Functions.

      Each thread has its own function.
  */
  std::vector<Function *> _functions;

  /// Function
  Function *_function;

  /** @name Parameters
   */
  ///@{

  /// Log context
  logging::LogContext *_log_context = nullptr;

  ///@}

  /// Set functions
  void set_functions(const std::vector<Function *>& functions) {
    assert(!functions.empty());
    assert(functions[0]);

    _functions = functions;
    _function = functions[0];
  }

public:

  /** Constructor.
      \param n Size of bit vectors
      \param num_objectives Number of objectives
  */
  Algorithm(int n, int num_objectives)
  {
    if (n <= 0)
      throw std::runtime_error("multiobjective::Algorithm::Algorithm: bit vector size must be positive");
    if (num_objectives <= 0)
      throw std::runtime_error("multiobjective::Algorithm::Algorithm: num_objectives size must be positive");
  }

  /// Destructor
  virtual ~Algorithm() {}

  /** @name Optimization
   */
  ///@{

  /// Minimize
  virtual void minimize(const std::vector<Function *>& functions) = 0;

  /// Get solutions
  virtual const Population& get_solutions() = 0;

  ///@}

  /// Set the log context
  void set_log_context(logging::LogContext *log_context) { _log_context = log_context; }
    
};


} // end of namespace algorithm
} // end of namespace multiobjective
} // end of namespace hnco


#endif
