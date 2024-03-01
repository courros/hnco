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


/// Algorithm decorator
class Decorator: public Algorithm {

protected:

  /// Decorated algorithm
  Algorithm *_algorithm;

public:

  /**
   * Constructor.
   *
   * The decorator itself is an algorithm created with the same bit
   * vector size as that of the decorated algorithm.
   *
   * @pre algorithm must be a pointer to a valid Algorithm.
   */
  Decorator(Algorithm *algorithm)
    : Algorithm(algorithm->get_bv_size())
    , _algorithm(algorithm)
  {}
    
};


} // end of namespace algorithm
} // end of namespace hnco


#endif
