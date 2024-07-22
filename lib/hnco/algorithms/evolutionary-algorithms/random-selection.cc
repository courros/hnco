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

#include <algorithm>		// std::max_element
#include <cassert>
#include <cmath>		// std::exp

#include "random-selection.hh"

using namespace hnco::algorithm;
using namespace hnco::function;
using namespace hnco::random;
using namespace hnco;

const bit_vector_t&
UniformSelection::select()
{
  return _population.bvs[_choose_individual(Generator::engine)];
}

void
TournamentSelection::init()
{
  _tournament_selection.set_tournament_size(_tournament_size);
  _tournament_selection.init();
}

const bit_vector_t&
TournamentSelection::select()
{
  return _tournament_selection.select();
}

void
FitnessProportionateSelection::init()
{
  std::discrete_distribution<>::param_type params(_population.values.begin(), _population.values.end());
  _distribution.param(params);
}

const bit_vector_t&
FitnessProportionateSelection::select()
{
  const int index = _distribution(Generator::engine);
  assert(is_in_range(index, _population.bvs.size()));
  return _population.bvs[index];
}

void
BoltzmannSelection::init()
{
  const auto& ys = _population.values;
  auto& zs = _exponentiated_fitnesses;

  auto iter = std::max_element(ys.begin(), ys.end());
  const double fmax = *iter;

  const int N = _population.get_size();
  for (int i = 0; i < N; i++) {
    zs[i] = std::exp(_beta * (ys[i] - fmax));
    assert(zs[i] <= 1);
  }

  std::discrete_distribution<>::param_type params(zs.begin(), zs.end());
  _distribution.param(params);
}
