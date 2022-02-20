/* Copyright (C) 2016, 2017, 2018, 2019, 2020, 2021, 2022 Arnaud Berny

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

#include "hnco/multiobjective/algorithms/all.hh"

#include "algorithm-factory.hh"


using namespace hnco::multiobjective::algorithm;
using namespace hnco::multiobjective::app;
using namespace hnco::random;
using namespace hnco;


Algorithm *
CommandLineAlgorithmFactory::make(int bv_size, int num_objectives)
{
  switch(_options.get_algorithm()) {

  case 1400: {
    auto algo = new Nsga2
      (bv_size,
       num_objectives,
       _options.get_ea_mu());

    algo->set_allow_no_mutation(_options.with_ea_allow_no_mutation());
    algo->set_crossover_probability(_options.get_ea_crossover_probability());
    algo->set_mutation_rate(_options.get_ea_mutation_rate() / bv_size);
    algo->set_num_iterations(_options.get_num_iterations());
    algo->set_tournament_size(_options.get_ea_tournament_size());

    return algo;
  }

  default:
    throw std::runtime_error("CommandLineAlgorithmFactory::make: Unknown algorithm type: " + std::to_string(_options.get_algorithm()));
  }
}
