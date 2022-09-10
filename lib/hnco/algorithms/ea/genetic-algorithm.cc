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

#include "genetic-algorithm.hh"


using namespace hnco::function;
using namespace hnco::random;
using namespace hnco::algorithm;
using namespace hnco;


void
GeneticAlgorithm::init()
{
  _mutation.set_mutation_rate(_mutation_rate);
  _mutation.set_allow_no_mutation(_allow_no_mutation);

  _do_crossover = std::bernoulli_distribution(_crossover_probability);

  _tournament_selection.set_tournament_size(_tournament_size);

  _parents.random();
  _parents.evaluate(_function);
  _parents.sort();

  set_solution(_parents.get_best_bv(),
               _parents.get_best_value());
}


void
GeneticAlgorithm::iterate()
{
  for (int i = 0; i < _offsprings.size(); i++) {
    auto& offspring = _offsprings.bvs[i];

    // Crossover
    if (_do_crossover(Generator::engine))
      _crossover.breed(_tournament_selection.select(), _tournament_selection.select(), offspring);
    else
      offspring = _tournament_selection.select();

    // Mutation
    _mutation.mutate(offspring);
  }

  if (_functions.size() > 1)
    _offsprings.evaluate_in_parallel(_functions);
  else
    _offsprings.evaluate(_function);

  _comma_selection.select();

  update_solution(_parents.get_best_bv(),
                  _parents.get_best_value());
}
