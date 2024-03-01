/* Copyright (C) 2016, 2017, 2018, 2019, 2020, 2021, 2022, 2023, 2024 Arnaud Berny

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

#ifndef HNCO_ALGORITHMS_DECORATORS_RESTART_H
#define HNCO_ALGORITHMS_DECORATORS_RESTART_H

#include "decorator.hh"


namespace hnco {
namespace algorithm {


/** %Restart.

    %Restart an algorithm an indefinite number of times. The %Restart
    decorator can be used in conjonction with OnBudgetFunction or
    StopOnMaximum.
*/
class Restart: public Decorator {

  /// Number of iterations
  int _num_iterations = 0;

  /**
   * Iterate.
   *
   * @param first_iteration Boolean which is true if this is the first
   * iteration.
   */
  void iterate(bool first_iteration);

public:

  /// Constructor
  Restart(Algorithm *algorithm)
    : Decorator(algorithm)
  {}

  /**
   * @name Optimization
   */
  ///@{

  /// Maximize
  void maximize(const std::vector<function::Function *>& functions) override;

  ///@}

  /** @name Setters
   */
  ///@{

  /**
   * Set the number of iterations.
   *
   * @param n Number of iterations
   * @warning n <= 0 means indefinite
   */
  void set_num_iterations(int n) { _num_iterations = n; }

  ///@}

};


} // end of namespace algorithm
} // end of namespace hnco


#endif
