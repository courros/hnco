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

#include <assert.h>
#include <math.h>               // std::log

#include <algorithm>            // std::fill, std::min, std::swap, std::min_element, std::all_of

#include "hnco/algorithms/probability-vector/probability-vector.hh"
#include "hnco/random.hh"

#include "mimic.hh"


using namespace hnco::algorithm;
using namespace hnco::function;
using namespace hnco::random;
using namespace hnco;


void
Mimic::init()
{
  assert(_selection_size > 0);
  assert(_selection_size < int(_population.get_size()));

  random_solution();
  perm_random(_permutation);
  for (auto& p: _parameters)
    std::fill(p.begin(), p.end(), 0.5);
}

void
Mimic::sample(bit_vector_t& bv)
{
  assert(bv.size() == _permutation.size());
  assert(bv.size() == _parameters[0].size());
  assert(bv.size() == _parameters[1].size());
  assert(perm_is_valid(_permutation));

  // First component
  bv[_permutation[0]] = random_bit(_parameters[1][0]);

  // Other components
  for (size_t i = 1; i < bv.size(); i++)
    bv[_permutation[i]] = random_bit(_parameters[bv[_permutation[i - 1]]][i]);
}

void
Mimic::compute_conditional_entropy(int index)
{
  assert(index >= 0);
  assert(index < int(_entropies.size()));

  double entropy = 0;
  int total = 0;

  for (size_t i = 0; i < _table.size(); i++)  {
    int count = _table[i][0] + _table[i][1];
    if (count) {
      if (_table[i][0])
        entropy += _table[i][0] * std::log(double(count) / _table[i][0]);
      if (_table[i][1])
        entropy += _table[i][1] * std::log(double(count) / _table[i][1]);
    }
    _parameters[i][index] = count ? _table[i][1] / double(count) : 0.5;
    total += count;
  }

  assert(total == _selection_size);
  assert(entropy >= 0);

  _entropies[index] = entropy;
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
  size_t root = std::min_element(_entropies.begin(), _entropies.end()) - _entropies.begin();
  assert(root < _permutation.size());

  perm_identity(_permutation);
  std::swap(_permutation[0], _permutation[root]);
  _parameters[0][0] = _mean[root];
  _parameters[1][0] = _mean[root];

  for (size_t i = 1; i < _permutation.size(); i++) {
    int first = _permutation[i - 1];

    for (size_t j = i; j < _permutation.size(); j++) {
      int second = _permutation[j];
      assert(second != first);

      _table.fill({0, 0});
      for (int k = 0; k < _selection_size; k++) {
        const bit_vector_t& bv = _population.get_best_bv(k);
        _table[bv[first]][bv[second]]++;
      }

      compute_conditional_entropy(j);
    }

    size_t child = std::min_element(_entropies.begin() + i, _entropies.end()) - _entropies.begin();
    assert(child < _permutation.size());

    if (child != i) {
      std::swap(_permutation[i], _permutation[child]);
      for (auto& p: _parameters)
        p[i] = p[child];
    }
    assert(perm_is_valid(_permutation));
  }

  for (auto& p: _parameters)
    pv_bound(p, _lower_bound, _upper_bound);

}

void
Mimic::iterate()
{
  for (int i = 0; i < _population.get_size(); i++)
    sample(_population.bvs[i]);

  if (_functions.size() > 1)
    _population.evaluate_in_parallel(_functions);
  else
    _population.evaluate(_function);

  _population.sort();

  update_solution(_population.get_best_bv(),
                  _population.get_best_value());

  update_model();
}
