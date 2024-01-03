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

#ifndef HNCO_ALGORITHMS_GOMEA_GOMEA_H
#define HNCO_ALGORITHMS_GOMEA_GOMEA_H

#include <memory>               // std::shared_ptr

#include "hnco/algorithms/algorithm.hh"
#include "hnco/functions/controllers/all.hh" // hnco::function::controller::ProgressTracker
#include "gomea/src/common/linkage_config.hpp"
#include "gomea/src/discrete/Config.hpp"

#include "hnco-fitness.hh"


namespace hnco {
namespace algorithm {

/// GOMEA
namespace gomea {


struct Implementation;

/** GOMEA.

    %Implemention of the Gene-pool Optimal Mixing Evolutionary Algorithm.

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

  /// Linkage configuration
  ::gomea::linkage_config_t _linkage_config;

  /// Configuration
  ::gomea::discrete::Config _config;

  /// Fitness
  std::shared_ptr<HncoFitness> _fitness;

  /// Progress tracker
  std::shared_ptr<hnco::function::controller::ProgressTracker> _tracker;

public:

  /// Constructor
  Gomea(int n): Algorithm(n) {}

  /// Maximize
  void maximize(const std::vector<function::Function *>& functions);

  /// Finalize
  void finalize();

};


} // end of namespace gomea
} // end of namespace algorithm
} // end of namespace hnco


#endif
