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

#ifndef HNCO_ALGORITHMS_FAST_EFFICIENT_P3_IMPLEMENTATION_H
#define HNCO_ALGORITHMS_FAST_EFFICIENT_P3_IMPLEMENTATION_H

#include <memory>               // std::shared_ptr

#include "fast-efficient-p3/Configuration.h"
#include "fast-efficient-p3/MiddleLayer.h"

#include "hnco-evaluator.hh"


namespace hnco {
namespace algorithm {
namespace fast_efficient_p3 {

  struct Implementation  {
    Configuration configuration;
    std::shared_ptr<HncoEvaluator> evaluator;
    std::shared_ptr<Middle_Layer> middle_layer;
  };

} // end of namespace fast_efficient_p3
} // end of namespace algorithm
} // end of namespace hnco


#endif
