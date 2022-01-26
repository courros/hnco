/* Copyright (C) 2016, 2017, 2018, 2019, 2020, 2021, 2022 Arnaud Berny

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

#include <iostream>

#include "hnco/exception.hh"
#include "hnco/logging/log-context.hh"
#include "hnco/multiobjective/functions/function.hh"

#include "candidate-set.hh"


namespace hnco {
namespace multiobjective {

/// Algorithms
namespace algorithm {


/** Abstract search algorithm.

    All algorithms minimize some given function.
*/
class Algorithm {

protected:

  /** Functions.

      Each thread has its own function.
  */
  std::vector<function::Function *> _functions;

  /// Function
  function::Function *_function;

  /** @name Parameters
   */
  ///@{

  /// Log context
  logging::LogContext *_log_context = nullptr;

  ///@}

  /// Set functions
  void set_functions(const std::vector<function::Function *>& functions) {
    assert(!functions.empty());
    assert(functions[0]);

    _functions = functions;
    _function = functions[0];
  }

  /// Get bit vector size
  int get_bv_size() { return _solution.first.size(); }

public:

  /// Destructor
  virtual ~Algorithm() {}

  /** @name Optimization
   */
  ///@{

  /// Minimize
  virtual void minimize(const std::vector<function::Function *>& functions) = 0;

  /** Finalize.

      Does nothing.

      It is usually overridden by algorithms which do not keep their
      solution up-to-date. In case _function throws a LastEvaluation
      exception, the algorithm might leave its solution in an
      undefined state. This can be fixed in this member function.
  */
  virtual void finalize() {}

  /// Get solutions
  virtual const CandidateSet& get_solutions() = 0;

  ///@}

  /// Set the log context
  void set_log_context(logging::LogContext *log_context) { _log_context = log_context; }
    
};


} // end of namespace algorithm
} // end of namespace multiobjective
} // end of namespace hnco


#endif
