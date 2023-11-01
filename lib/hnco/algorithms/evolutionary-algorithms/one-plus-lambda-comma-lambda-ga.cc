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

#include "one-plus-lambda-comma-lambda-ga.hh"


using namespace hnco::function;
using namespace hnco::random;
using namespace hnco::algorithm;
using namespace hnco;


void
OnePlusLambdaCommaLambdaGa::init()
{
  _radius_dist = std::binomial_distribution<int>(_parent.size(), _mutation_rate);
  _crossover.set_bias(_crossover_bias);

  random_solution();
}


void
OnePlusLambdaCommaLambdaGa::iterate()
{
  // Mutation
  _mutation.set_radius(_radius_dist(Generator::engine));
  for (int i = 0; i < _offsprings.get_size(); i++) {
    auto& offspring = _offsprings.bvs[i];
    _mutation.map(_solution.first, offspring);
  }
  _offsprings.evaluate(_function);
  _offsprings.sort();

  double value = _offsprings.get_best_value(0);
  int n = 0;
  while (_offsprings.get_best_value(n) == value && n < _offsprings.get_size())
    n++;
  assert(n > 0);
  std::uniform_int_distribution<int> choose_parent(0, n - 1);
  _parent = _offsprings.get_best_bv(choose_parent(Generator::engine));

  // Crossover
  for (int i = 0; i < _offsprings.get_size(); i++)
    _crossover.recombine(_solution.first, _parent, _offsprings.bvs[i]);
  _offsprings.evaluate(_function);
  _offsprings.sort();

  // Selection
  value = _offsprings.get_best_value(0);
  if (value >= _solution.second) {
    n = 0;
    while (_offsprings.get_best_value(n) == value && n < _offsprings.get_size())
      n++;
    assert(n > 0);
    choose_parent = std::uniform_int_distribution<int>(0, n - 1);

    _solution.first = _offsprings.get_best_bv(choose_parent(Generator::engine));
    _solution.second = value;
  }

}
