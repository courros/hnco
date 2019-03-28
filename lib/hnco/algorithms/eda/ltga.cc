/* Copyright (C) 2016, 2017, 2018, 2019 Arnaud Berny

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

#include <memory>               // std::shared_ptr

#include "hnco/random.hh"

#include "fast-efficient-p3/Configuration.h"
#include "fast-efficient-p3/MiddleLayer.h"
#include "fast-efficient-p3/LTGA.h"

#include "ltga.hh"
#include "hnco-evaluator.hh"

using namespace hnco::algorithm::eda;
using namespace hnco::algorithm;
using namespace hnco;


void
Ltga::maximize()
{
  Configuration configuration;

  configuration.set("binary_insert", 1);
  configuration.set("cluster_ordering", std::string("least_linked_first"));
  configuration.set("disable_solution_outfile", 1);
  configuration.set("donate_until_different", 0);
  configuration.set("hill_climber", std::string("no_action"));
  configuration.set("keep_zeros", 0);
  configuration.set("no_singles", 0);
  configuration.set("precision", 65536);
  configuration.set("restrict_cluster_size", 0);
  configuration.set("solution_file", std::string("hboa-solution.txt"));
  configuration.set("verbosity", 0);

  configuration.set("length", _solution.first.size());
  configuration.set("pop_size", _population_size);

  std::shared_ptr<HncoEvaluator> evaluator(new HncoEvaluator(_function));
  std::shared_ptr<Middle_Layer> middle_layer(new Middle_Layer(configuration, evaluator));

  LTGA ltga(hnco::random::Random::generator, middle_layer, configuration);

  while (ltga.iterate()) {}

  bv_from_vector_bool(_solution.first, middle_layer->best_solution);
  _solution.second = middle_layer->best_fitness;
}
