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

#include "hnco/random.hh"

#include "gomea/src/discrete/Config.hpp"
#include "gomea/src/discrete/gomeaIMS.hpp"

#include "hnco-fitness.hh"
#include "implementation.hh"
#include "gomea.hh"

using namespace hnco::function::controller;
using namespace hnco::algorithm::gomea;
using namespace hnco::algorithm;
using namespace hnco;


Gomea::Gomea(int n):
  Algorithm(n)
{
  _implementation = new Implementation();
}


Gomea::~Gomea()
{
  delete _implementation;
}


void
Gomea::maximize(const std::vector<function::Function *>& functions)
{
  set_functions(functions);

  _implementation->tracker = std::make_shared<ProgressTracker>(_function);
  _implementation->tracker->set_record_bit_vector(true);

  _implementation->fitness               = std::make_shared<HncoFitness>(_implementation->tracker.get());
  _implementation->linkage_config.type   = ::gomea::linkage::LINKAGE_TREE;
  _implementation->config.fitness        = _implementation->fitness.get();
  _implementation->config.linkage_config = &_implementation->linkage_config;

  // _implementation->config.folder                     = "output_discrete_gomea";
  // _implementation->config.maximumNumberOfGOMEAs      = max_number_of_populations;
  // _implementation->config.IMSsubgenerationFactor     = IMS_subgeneration_factor;
  // _implementation->config.basePopulationSize         = base_population_size;
  // _implementation->config.maxArchiveSize             = 1000 ;
  // _implementation->config.maximumNumberOfEvaluations = max_number_of_evaluations;
  // _implementation->config.maximumNumberOfGenerations = max_number_of_generations;
  // _implementation->config.maximumNumberOfSeconds     = max_number_of_seconds;
  // _implementation->config.randomSeed                 = random_seed;
  // _implementation->config.fix_seed                   = true;

  ::gomea::discrete::gomeaIMS gomea(&_implementation->config);
  gomea.run();

}


void
Gomea::finalize()
{
  ProgressTracker::Event last_improvement = _implementation->tracker->get_last_improvement();
  _solution = last_improvement.solution;
}
