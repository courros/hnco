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

#ifndef HNCO_ALGORITHMS_GOMEA_IMPLEMENTATION_H
#define HNCO_ALGORITHMS_GOMEA_IMPLEMENTATION_H

#include <memory>               // std::shared_ptr

#include "hnco/functions/controllers/all.hh" // hnco::function::controller::ProgressTracker
#include "gomea/src/discrete/Config.hpp"

#include "hnco-fitness.hh"


namespace hnco {
namespace algorithm {
namespace gomea {

  /// %Implementation
  struct Implementation  {

    /// Configuration
    ::gomea::discrete::Config configuration;

    /// Fitness
    std::shared_ptr<HncoFitness> fitness;

    /// Progress tracker
    std::shared_ptr<hnco::function::controller::ProgressTracker> tracker;

  };

} // end of namespace gomea
} // end of namespace algorithm
} // end of namespace hnco


#endif
