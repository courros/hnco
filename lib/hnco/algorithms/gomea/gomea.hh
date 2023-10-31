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

#ifndef HNCO_ALGORITHMS_GOMEA_GOMEA_H
#define HNCO_ALGORITHMS_GOMEA_GOMEA_H

#include <memory>               // std::unique_ptr

#include "hnco/algorithms/algorithm.hh"


namespace hnco {
namespace algorithm {

/// GOMEA
namespace gomea {


struct Implementation;

/** GOMEA.

    %Implemention of the GOMEA.

    Author: Anton Bouter

    Integrated into HNCO by Arnaud Berny

    References:

    - A Joint Python/C++ Library for Efficient yet Accessible
      Black-Box and Gray-Box Optimization with GOMEA, Anton Bouter and
      Peter A.N. Bosman

    - Parameterless Gene-pool Optimal Mixing Evolutionary Algorithms,
      Arkadiy Dushatskiy, Marco Virgolin, Anton Bouter, Dirk Thierens,
      and Peter A. N. Bosman

*/
class Gomea: public Algorithm {

  /** Pointer to implementation.

      The main motivation for this pattern is to avoid including
      declarations from gomea into the global namespace.

      A raw pointer is used instead of a unique_ptr because the latter
      will not compile with pybind11.
  */
  Implementation *_implementation;;

public:

  /// Constructor
  Gomea(int n);

  /// Destructor
  ~Gomea();

  /// Maximize
  void maximize(const std::vector<function::Function *>& functions);

  /// Finalize
  void finalize();

};


} // end of namespace gomea
} // end of namespace algorithm
} // end of namespace hnco


#endif
