/* Copyright (C) 2016, 2017, 2018 Arnaud Berny

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

#include <algorithm>            // std::fill, std::min, std::swap, std::min_element, std::all_of

#include <math.h>               // std::log

#include "hnco/algorithms/pv/probability-vector.hh"
#include "hnco/random.hh"

#include "mimic.hh"


using namespace hnco::algorithm::eda;
using namespace hnco::function;
using namespace hnco::random;
using namespace hnco;


void
Mimic::init()
{
  random_solution();
  perm_random(_permutation);
  std::fill(_parameters.begin(), _parameters.end(), std::make_pair(0.5, 0.5));
}

void
Mimic::sample(bit_vector_t& bv)
{
  assert(bv.size() == _permutation.size());
  assert(bv.size() == _parameters.size());
  assert(perm_is_valid(_permutation));

  // First component
  bv[_permutation[0]] = (Random::uniform() < _parameters[0].first) ? 1 : 0;

  // Other components
  for (std::size_t i = 1; i < bv.size(); i++)
    if (bv[_permutation[i - 1]])
      bv[_permutation[i]] = (Random::uniform() < _parameters[i].first) ? 1 : 0;
    else
      bv[_permutation[i]] = (Random::uniform() < _parameters[i].second) ? 1 : 0;
}

void
Mimic::compute_conditional_entropy(std::size_t j)
{
  double entropy = 0;

  int c0 = _table[0][0] + _table[0][1];
  if (c0) {
    if (_table[0][0])
      entropy += _table[0][0] * std::log(double(c0) / _table[0][0]);
    if (_table[0][1])
      entropy += _table[0][1] * std::log(double(c0) / _table[0][1]);
  }
  _parameters[j].second = c0 ? _table[0][1] / double(c0) : 0.5;

  int c1 = _table[1][0] + _table[1][1];
  if (c1) {
    if (_table[1][0])
      entropy += _table[1][0] * std::log(double(c1) / _table[1][0]);
    if (_table[1][1])
      entropy += _table[1][1] * std::log(double(c1) / _table[1][1]);
  }
  _parameters[j].first = c1 ? _table[1][1] / double(c1) : 0.5;

  assert(c0 + c1 == _selection_size);
  assert(entropy >= 0);

  _entropies[j] = entropy;
}

void
Mimic::update_model()
{
  pv_init(_mean);
  for (int i = 0; i < _selection_size; i++)
    pv_add(_mean, _population.get_best_bv(i));
  pv_average(_mean, _selection_size);

  /* Choose component (root) with the smallest entropy or with
     probability closest to 0 or 1 */

  std::transform(_mean.begin(), _mean.end(), _entropies.begin(), [](double x){ return std::min(x, 1 - x); });
  assert(std::all_of(_entropies.begin(), _entropies.end(), [](double x){ return x >= 0; }));
  std::size_t root = std::min_element(_entropies.begin(), _entropies.end()) - _entropies.begin();
  assert(root < _permutation.size());

  perm_identity(_permutation);
  std::swap(_permutation[0], _permutation[root]);
  _parameters[0].first = _mean[root];
  _parameters[0].second = _mean[root];

  for (std::size_t i = 1; i < _permutation.size(); i++) {
    std::size_t first = _permutation[i - 1];

    for (std::size_t j = i; j < _permutation.size(); j++) {
      std::size_t second = _permutation[j];
      assert(second != first);

      _table.fill({0, 0});
      for (int k = 0; k < _selection_size; k++) {
        const bit_vector_t& bv = _population.get_best_bv(k);
        _table[bv[first]][bv[second]]++;
      }

      compute_conditional_entropy(j);
    }

    std::size_t child = std::min_element(_entropies.begin() + i, _entropies.end()) - _entropies.begin();
    assert(child < _permutation.size());

    if (child != i) {
      std::swap(_permutation[i], _permutation[child]);
      std::swap(_parameters[i], _parameters[child]);
    }
    assert(perm_is_valid(_permutation));
  }
}

void
Mimic::iterate()
{
  for (size_t i = 0; i < _population.size(); i++)
    sample(_population.get_bv(i));

  if (_functions.size() > 1)
    _population.eval(_functions);
  else
    _population.eval(_function);

  _population.sort();

  update_solution(_population.get_best_bv(),
                  _population.get_best_value());

  update_model();
}
