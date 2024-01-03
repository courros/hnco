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

#include "gomea/src/discrete/Config.hpp"
#include "gomea/src/discrete/gomeaIMS.hpp"

#include "gomea.hh"

using namespace hnco::function::controller;
using namespace hnco::algorithm::gomea;
using namespace hnco::algorithm;
using namespace hnco;


void
Gomea::maximize(const std::vector<function::Function *>& functions)
{
  set_functions(functions);

  _tracker = std::make_shared<ProgressTracker>(_function);
  _tracker->set_record_bit_vector(true);

  _fitness = std::make_shared<HncoFitness>(_tracker.get());

  _linkage_config.type = ::gomea::linkage::LINKAGE_TREE;

  _config.fitness        = _fitness.get();
  _config.linkage_config = &_linkage_config;

  // _config.folder                     = "output_discrete_gomea";
  // _config.maximumNumberOfGOMEAs      = max_number_of_populations;
  // _config.IMSsubgenerationFactor     = IMS_subgeneration_factor;
  // _config.basePopulationSize         = base_population_size;
  // _config.maxArchiveSize             = 1000 ;
  // _config.maximumNumberOfEvaluations = max_number_of_evaluations;
  // _config.maximumNumberOfGenerations = max_number_of_generations;
  // _config.maximumNumberOfSeconds     = max_number_of_seconds;
  // _config.randomSeed                 = random_seed;
  // _config.fix_seed                   = true;

  ::gomea::discrete::gomeaIMS gomea(&_config);
  gomea.run();
}


void
Gomea::finalize()
{
  ProgressTracker::Event last_improvement = _tracker->get_last_improvement();
  _solution = last_improvement.solution;
}
