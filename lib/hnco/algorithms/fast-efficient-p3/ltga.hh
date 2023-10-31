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

#ifndef HNCO_ALGORITHMS_FAST_EFFICIENT_P3_LTGA_H
#define HNCO_ALGORITHMS_FAST_EFFICIENT_P3_LTGA_H

#include <memory>               // std::unique_ptr

#include "hnco/algorithms/algorithm.hh"


namespace hnco {
namespace algorithm {
namespace fast_efficient_p3 {


struct Implementation;

/** Linkage Tree Genetic %Algorithm.

    %Implementation of the Linkage Tree Genetic %Algorithm.

    Author: Brian W. Goldman

    Integrated into HNCO by Arnaud Berny

    Reference:

    "Hierarchical problem solving with the linkage tree genetic
    algorithm" by D. Thierens and P. A. N. Bosman

*/
class Ltga: public Algorithm {

  /** Pointer to implementation.

      The main motivation for this pattern is to avoid including
      declarations from fast_efficient_p3 into the global namespace.

      A raw pointer is used instead of a unique_ptr because the latter
      will not compile with pybind11.
  */
  Implementation *_implementation;;

  /// Population size
  int _population_size = 10;

public:

  /// Constructor
  Ltga(int n);

  /// Destructor
  ~Ltga();

  /// Maximize
  void maximize(const std::vector<function::Function *>& functions);

  /// Finalize
  void finalize();

  /// Set population size
  void set_population_size(int n) { _population_size = n; }

};


} // end of namespace fast_efficient_p3
} // end of namespace algorithm
} // end of namespace hnco


#endif
