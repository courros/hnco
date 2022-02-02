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

#ifndef HNCO_MULTIOBJECTIVE_ALGORITHMS_ITERATIVE_ALGORITHM_H
#define HNCO_MULTIOBJECTIVE_ALGORITHMS_ITERATIVE_ALGORITHM_H

#include "algorithm.hh"


namespace hnco {
namespace multiobjective {
namespace algorithm {


/// Iterative algorithm
class IterativeAlgorithm: public Algorithm {

protected:

  /// Current iteration
  int _iteration;

  /// Last iteration
  bool _last_iteration = false;

  /// Something to log
  bool _something_to_log = false;

  /** @name Parameters
   */
  ///@{

  /// Number of iterations
  int _num_iterations = 0;

  ///@}

  /** @name Loop
   */
  ///@{

  /// Initialize
  virtual void init() {}

  /// Single iteration
  virtual void iterate() = 0;

  /// Log
  virtual void log() {}

  /** Loop.

      Calls init() then enter the main loop which, at each iteration,
      calls iterate() then log() only if _something_to_log is true.
  */
  virtual void loop() final;

  ///@}


public:

  /** Constructor.
      \param n Size of bit vectors
      \param num_objectives Number of objectives
  */
  IterativeAlgorithm(int n, int num_objectives)
    : Algorithm(n, num_objectives)
  {}

  /** @name Optimization
   */
  ///@{

  /** Minimize.

      Calls set_functions() then loop.
  */
  void minimize(const std::vector<Function *>& functions) override;

  ///@}

  /** @name Setters
   */
  ///@{

  /** Set the number of iterations.

      \param n Number of iterations

      \warning n <= 0 means indefinite
  */
  void set_num_iterations(int n) { _num_iterations = n; }

  ///@}

};


} // end of namespace algorithm
} // end of namespace multiobjective
} // end of namespace hnco


#endif
