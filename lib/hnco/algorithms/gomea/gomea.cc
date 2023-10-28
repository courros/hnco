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

#include "hnco-fitness.hh"
#include "implementation.hh"
#include "gomea.hh"

using namespace hnco::algorithm::gomea;
using namespace hnco::algorithm;
using namespace hnco;


Gomea::Gomea(int n):
  Algorithm(n)
{
  _pimpl = new Implementation();
}


Gomea::~Gomea()
{
  delete _pimpl;
}


void
Gomea::maximize(const std::vector<function::Function *>& functions)
{
  set_functions(functions);

  _pimpl->configuration.set("cluster_ordering", std::string("least_linked_first"));
  _pimpl->configuration.set("disable_solution_outfile", 1);
  _pimpl->configuration.set("donate_until_different", 0);
  _pimpl->configuration.set("hill_climber", std::string("no_action"));
  _pimpl->configuration.set("keep_zeros", 0);
  _pimpl->configuration.set("no_singles", 0);
  _pimpl->configuration.set("only_add_improvements", 1);
  _pimpl->configuration.set("precision", 65536);
  _pimpl->configuration.set("restrict_cluster_size", 0);
  _pimpl->configuration.set("solution_file", std::string("p3-solution.txt"));
  _pimpl->configuration.set("verbosity", 0);
  _pimpl->configuration.set("wait_until_k_modeled", 0);
  _pimpl->configuration.set("prevent_duplicates", 1);
  _pimpl->configuration.set("length", get_bv_size());
  _pimpl->evaluator = std::make_shared<HncoEvaluator>(_function);
  _pimpl->middle_layer = std::make_shared<Middle_Layer>(_pimpl->configuration,
                                                        _pimpl->evaluator);
  Pyramid pyramid(hnco::random::Generator::engine,
                  _pimpl->middle_layer,
                  _pimpl->configuration);
  while (pyramid.iterate()) {}
}


void
Gomea::finalize()
{
  bv_from_vector_bool(_solution.first, _pimpl->middle_layer->best_solution);
  _solution.second = _pimpl->middle_layer->best_fitness;
}
