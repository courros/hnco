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

#include "hnco/random.hh"

#include "fast-efficient-p3/Configuration.h"
#include "fast-efficient-p3/MiddleLayer.h"
#include "fast-efficient-p3/HBOA.h"

#include "hnco-evaluator.hh"
#include "implementation.hh"
#include "hboa.hh"

using namespace hnco::algorithm::fast_efficient_p3;
using namespace hnco::algorithm;
using namespace hnco;


Hboa::Hboa(int n):
  Algorithm(n)
{
  _implementation = new Implementation();
}


Hboa::~Hboa()
{
  delete _implementation;
}


void
Hboa::maximize(const std::vector<function::Function *>& functions)
{
  set_functions(functions);

  _implementation->configuration.set("verbosity", 0);
  _implementation->configuration.set("solution_file", std::string("hboa-solution.txt"));
  _implementation->configuration.set("disable_solution_outfile", 1);
  _implementation->configuration.set("length", get_bv_size());
  _implementation->configuration.set("pop_size", _population_size);
  _implementation->configuration.set("hill_climber", std::string("no_action"));
  _implementation->evaluator = std::make_shared<HncoEvaluator>(_function);
  _implementation->middle_layer = std::make_shared<Middle_Layer>(_implementation->configuration,
                                                                 _implementation->evaluator);

  HBOA hboa(hnco::random::Generator::engine,
            _implementation->middle_layer,
            _implementation->configuration);
  while (hboa.iterate()) {}
}


void
Hboa::finalize()
{
  bv_from_vector_bool(_solution.first, _implementation->middle_layer->best_solution);
  _solution.second = _implementation->middle_layer->best_fitness;
}
