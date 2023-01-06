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
  perm_identity(_permutation);

  _selection_by_front_distance_pair.set_tournament_size(_tournament_size);

  _mutation.set_mutation_rate(_mutation_rate);
  _mutation.set_allow_no_mutation(_allow_no_mutation);

  _parents.random();
  _parents.evaluate(_function);

  std::vector<int> fronts(_parents.get_size());
  Nsga2ParetoFrontComputation computation(_parents);
  computation.compute(fronts);

  // Offsprings
  TournamentSelection<int, std::less<int>> selection(_parents.bvs, fronts);
  selection.set_tournament_size(_tournament_size);
  selection.init();
  for (int i = 0; i < _offsprings.get_size(); i++) {
    bit_vector_t& offspring = _offsprings.bvs[i];
    if (_do_crossover(Generator::engine))
      _crossover.recombine(selection.select(),selection.select(), offspring);
    else
      offspring = selection.select();
    _mutation.mutate(offspring);
  }

}

void
Nsga2::iterate()
{
  const int population_size = _parents.get_size();
  const int num_objectives = _function->get_output_size();

  _offsprings.evaluate(_function);

  // Build full population (parents + offsprings)
  for (int i = 0; i < population_size; i++) {
    std::swap(_parents.bvs[i], _full_population.bvs[i]);
    std::swap(_parents.values[i], _full_population.values[i]);
    std::swap(_offsprings.bvs[i], _full_population.bvs[population_size + i]);
    std::swap(_offsprings.values[i], _full_population.values[population_size + i]);
  }

  _pareto_front_computation.compute(_pareto_fronts);

  // Sort _permutation by increasing Pareto front
  {
    auto compare = [this](int i, int j){ return this->_pareto_fronts[i] < this->_pareto_fronts[j]; };
    perm_random(_permutation);
    std::sort(_permutation.begin(), _permutation.end(), compare);
  }

  const int last_front = _pareto_fronts[_permutation[population_size - 1]];

  // Compute crowding distances
  std::fill(_crowding_distances.begin(), _crowding_distances.end(), 0);
  auto start = _permutation.begin();
  for (int front = 0; front <= last_front; front++) {
    assert(_pareto_fronts[*start] == front);
    auto predicate = [this, front](int i){ return this->_pareto_fronts[i] == front; };
    const auto stop = std::find_if_not(start + 1, _permutation.end(), predicate);
    for (int k = 0; k < num_objectives; k++) {
      auto compare = [this, k](int i, int j){ return this->_full_population.values[i][k] < this->_full_population.values[j][k]; };
      std::sort(start, stop, compare);
      static_assert(std::numeric_limits<double>::has_infinity == true);
      _crowding_distances[*start] = std::numeric_limits<double>::infinity();
      _crowding_distances[*(stop - 1)] = std::numeric_limits<double>::infinity();
      if (std::distance(start, stop) >= 3) {
        for (auto iter = start + 1; iter != stop - 1; iter++) {
          assert(_full_population.values[*(iter + 1)][k] - _full_population.values[*(iter - 1)][k] >= 0);
          _crowding_distances[*iter] += _full_population.values[*(iter + 1)][k] - _full_population.values[*(iter - 1)][k];
        }
      }
    }
    start = stop;
  }

  // Sort last front by decreasing crowding distance
  {
    auto predicate = [this, last_front](int i){ return this->_pareto_fronts[i] == last_front; };
    start = std::find_if(_permutation.begin(), _permutation.end(), predicate);
    const auto stop = std::find_if_not(start + 1, _permutation.end(), predicate);
    auto compare = [this](int i, int j){ return this->_crowding_distances[i] > this->_crowding_distances[j]; };
    std::sort(start, stop, compare);
  }

  // Build parent population
  for (int i = 0; i < population_size; i++) {
    const int index = _permutation[i];
    std::swap(_parents.bvs[i], _full_population.bvs[index]);
    std::swap(_parents.values[i], _full_population.values[index]);
    _front_distance_pairs[i].pareto_front = _pareto_fronts[index];
    _front_distance_pairs[i].crowding_distance = _crowding_distances[index];
  }

  // Offsprings
  _selection_by_front_distance_pair.init();
  for (int i = 0; i < _offsprings.get_size(); i++) {
    bit_vector_t& offspring = _offsprings.bvs[i];
    if (_do_crossover(Generator::engine))
      _crossover.recombine(_selection_by_front_distance_pair.select(),_selection_by_front_distance_pair.select(), offspring);
    else
      offspring = _selection_by_front_distance_pair.select();
    _mutation.mutate(offspring);
  }

}

void
Nsga2::finalize()
{
  std::vector<int> fronts(_parents.get_size());
  Nsga2ParetoFrontComputation computation(_parents);
  computation.compute(fronts);

  int index = 0;
  for (int i = 0; i < _parents.get_size(); i++) {
    if (fronts[i] == 0) {
      _solutions.bvs[index] = _parents.bvs[i];
      _solutions.values[index] = _parents.values[i];
      index++;
    }
  }

  _solutions.shrink(index);
}
