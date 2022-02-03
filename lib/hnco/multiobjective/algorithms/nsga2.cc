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

#include <algorithm>            // std::find_if, std::find_if_not, std::fill
#include <limits>               // std::numeric_limits
#include <utility>              // std::swap
#include <iterator>             // std::distance

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
    const int last_front = fronts[before];
    auto predicate = [&, last_front](int i){ return fronts[i] == last_front; };
    const auto start = std::find_if(indices.begin(), indices.end(), predicate);
    assert(start != indices.end());
    const auto stop = std::find_if_not(start + 1, indices.end(), predicate);

    // Compute crowding distance
    std::fill(_crowding_distance.begin(), _crowding_distance.end(), 0);
    const int num_objectives = _augmented_population.values[0].size();
    for (int k = 0; k < num_objectives; k++) {
      auto compare = [this, k](int i, int j){ return this->_augmented_population.values[i][k] < this->_augmented_population.values[j][k]; };
      std::sort(start, stop, compare);
      _crowding_distance[*start] = std::numeric_limits<double>::infinity();
      _crowding_distance[*(stop - 1)] = std::numeric_limits<double>::infinity();
      if (std::distance(start, stop) >= 3) {
        for (auto iter = start + 1; iter != stop - 1; iter++)
          _crowding_distance[*iter] += _augmented_population.values[*(iter + 1)][k] - _augmented_population.values[*(iter - 1)][k];
      }
    }

    auto compare = [this](int i, int j){ return this->_crowding_distance[i] > this->_crowding_distance[j]; };
    std::sort(start, stop, compare);
  }

  // Build parent population
  for (int i = 0; i < _parents.size(); i++) {
    const int index = _augmented_population.indices[i];
    std::swap(_parents.bvs[i], _augmented_population.bvs[index]);
    std::swap(_parents.values[i], _augmented_population.values[index]);
  }
}
