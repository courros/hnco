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

#include <algorithm>            // std::find_if
#include <utility>              // std::swap

#include "hnco/random.hh"       // hnco::random::Generator::engine

#include "nsga2.hh"

using namespace hnco::multiobjective::algorithm;
using namespace hnco::random;


void
Nsga2::init()
{
  _mutation.set_mutation_rate(_mutation_rate);
  _mutation.set_allow_no_mutation(_allow_no_mutation);

  _parents.random();
  _parents.evaluate(_function);
}

void
Nsga2::iterate()
{
  const int population_size = _parents.size();

  // Offsprings
  for (int i = 0; i < _offsprings.size(); i++) {
    bit_vector_t& offspring = _offsprings.bvs[i];
    if (_do_crossover(Generator::engine))
      _crossover.breed(_selection.select(),_selection.select(), offspring);
    else
      offspring = _selection.select();
    _mutation.mutate(offspring);
  }
  _offsprings.evaluate(_function);

  // Build augmented population (parents + offsprings)
  for (int i = 0; i < _parents.size(); i++) {
    std::swap(_parents.bvs[i], _augmented_population.bvs[i]);
    std::swap(_parents.values[i], _augmented_population.values[i]);
    std::swap(_offsprings.bvs[i], _augmented_population.bvs[population_size + i]);
    std::swap(_offsprings.values[i], _augmented_population.values[population_size + i]);
  }

  _non_domination_sort.sort();
  // ensure _augmented_population.pareto_fronts is correct
  // ensure _augmented_population.indices is correct

  auto& fronts = _augmented_population.pareto_fronts;
  auto& indices = _augmented_population.indices;

  int before = indices[population_size - 1];
  int after = indices[population_size];
  // Check for last front overflowing population_size
  if (fronts[before] == fronts[after]) {
    int last_front = fronts[before];
    // Check capture of fronts as a reference
    auto predicate = [fronts, last_front](int i){ return fronts[i] == last_front; };
    auto start = std::find_if(indices.begin(), indices.end(), predicate);
    assert(start != indices.end());
    auto stop = std::find_if_not(start + 1, indices.end(), predicate);
    for (auto iter = start; iter != stop; iter++) {
      // Compute _crowding_distance
    }
    auto compare = [this](int i, int j){ return this->_crowding_distance[i] > this->_crowding_distance[j]; };
    std::sort(start, stop, compare);
  }

  // Build parent population
  for (int i = 0; i < _parents.size(); i++) {
    int index = _augmented_population.indices[i];
    std::swap(_parents.bvs[i], _augmented_population.bvs[index]);
    std::swap(_parents.values[i], _augmented_population.values[index]);
  }
}
