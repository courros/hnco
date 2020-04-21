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

#include "crossover.hh"


using namespace hnco::algorithm;
using namespace hnco::random;
using namespace hnco;


void
UniformCrossover::breed(const bit_vector_t& parent1,
                        const bit_vector_t& parent2, bit_vector_t& offspring)
{
  assert(offspring.size() == parent1.size());
  assert(offspring.size() == parent2.size());

  for (size_t i = 0; i < offspring.size(); i++)
    if (Random::bernoulli())
      offspring[i] = parent1[i];
    else
      offspring[i] = parent2[i];
}


void
BiasedCrossover::breed(const bit_vector_t& parent1,
                       const bit_vector_t& parent2, bit_vector_t& offspring)
{
  assert(offspring.size() == parent1.size());
  assert(offspring.size() == parent2.size());

  for (size_t i = 0; i < offspring.size(); i++)
    if (_bernoulli_dist(Random::generator))
      offspring[i] = parent2[i];
    else
      offspring[i] = parent1[i];
}
