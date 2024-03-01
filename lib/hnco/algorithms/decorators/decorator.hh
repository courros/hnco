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

#ifndef HNCO_ALGORITHMS_DECORATORS_DECORATOR_H
#define HNCO_ALGORITHMS_DECORATORS_DECORATOR_H

#include "hnco/algorithms/algorithm.hh"


namespace hnco {
namespace algorithm {


/** Algorithm decorator.
 *
 * By default, an algorithm decorator delegates everything to the
 * decorated algorithm.
 */
class Decorator: public Algorithm {

protected:

  /// Decorated algorithm
  Algorithm *_algorithm;

public:

  /** Constructor.
   *
   * The Decorator itself is an Algorithm created with the same bit
   * vector size as that of the decorated algorithm.
   *
   * @pre Parameter a must be a pointer to a valid Algorithm.
   */
  Decorator(Algorithm *a)
    : Algorithm(a->get_bv_size())
    , _algorithm(a)
  {}

  /** @name Optimization
   */
  ///@{

  /// Maximize
  void maximize(const std::vector<function::Function *>& functions) override { _algorithm->maximize(functions); }

  /// Finalize
  void finalize() override { _algorithm->finalize(); }

  /// Get the solution
  const solution_t& get_solution() override { return _algorithm->get_solution(); }

  ///@}
    
};


} // end of namespace algorithm
} // end of namespace hnco


#endif
