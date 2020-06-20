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

#ifndef HNCO_ALGORITHMS_ITERATIVE_ALGORITHM_H
#define HNCO_ALGORITHMS_ITERATIVE_ALGORITHM_H

#include "algorithm.hh"


namespace hnco {
namespace algorithm {


/// Iterative search
class IterativeAlgorithm: public Algorithm {

protected:

  /// Current iteration
  int _iteration;

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
  virtual void init() = 0;

  /// Single iteration
  virtual void iterate() = 0;

  /// Log
  virtual void log() {}

  ///@}

public:

  /** Constructor.
      \param n Size of bit vectors */
  IterativeAlgorithm(int n): Algorithm(n) {}

  /** @name Optimization
   */
  ///@{

  /** Maximize.

      It is essentially a loop which, at each iteration, calls
      iterate() then log() only if _something_to_log is true.

      \warning If an exception such as LocalMaximumFound is thrown
      by iterate(), log() will not be called.
  */
  void maximize();

  ///@}

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
