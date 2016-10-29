/* Copyright (C) 2016 Arnaud Berny

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

#include <assert.h>

#include <iostream>

#include "hnco/random.hh"

#include "genetic-algorithm.hh"


using namespace hnco::function;
using namespace hnco::random;
using namespace hnco::algorithm;
using namespace hnco;
using namespace std;


void
uniform_crossover(const bit_vector_t& parent1,
                  const bit_vector_t& parent2, bit_vector_t& offspring)
{
  assert(offspring.size() == parent1.size());
  assert(offspring.size() == parent2.size());

  for (size_t i = 0; i < offspring.size(); i++)
    if (Random::random_bit())
      offspring[i] = parent1[i];
    else
      offspring[i] = parent2[i];

}


void
GeneticAlgorithm::init()
{
  _parents.random();
  _parents.eval(_function);
  _parents.sort();

  set_solution(_parents.get_nth_bv(0),
               _parents.get_evaluation(0).value);
}


void
GeneticAlgorithm::iterate()
{
  for (size_t i = 0; i < _offsprings.size(); i++) {
    bit_vector_t& offspring = _offsprings.get_bv(i);

    // Crossover
    if (_do_crossover(Random::engine))
      uniform_crossover(_parents.select(), _parents.select(), offspring);
    else
      offspring = _parents.select();

    // Mutation
    for (size_t j = 0; j < offspring.size(); j++)
      if (_do_mutation(Random::engine))
        bv_flip(offspring, j);
  }
  _offsprings.eval(_function);
  _offsprings.sort();

  _parents.comma_selection(_offsprings);
  update_solution(_parents.get_nth_bv(0),
                  _parents.get_evaluation(0).value);
}
