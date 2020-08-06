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

#include <assert.h>

#include <algorithm>            // std::find

#include "neighborhood.hh"


using namespace hnco::neighborhood;
using namespace hnco::random;
using namespace hnco;


void
MultiBitFlip::bernoulli_trials(int k)
{
  int n = _candidate.size();

  _flipped_bits.clear();
  for (size_t i = 0; i < _candidate.size(); i++) {
    double p = double(k) / double(n);
    if (Generator::uniform() < p) {
      _flipped_bits.push_back(i);
      k--;
    }
    n--;
  }
  assert(k == 0);
}


void
MultiBitFlip::rejection_sampling(int k)
{
  _flipped_bits.clear();
  for (int i = 0; i < k; i++) {
    int index;
    do {
      index = _index_dist(Generator::engine);
    } while (std::find(begin(_flipped_bits), end(_flipped_bits), index) != _flipped_bits.end());
    _flipped_bits.push_back(index);
  }
  std::sort(begin(_flipped_bits), end(_flipped_bits));
}


void
StandardBitMutation::sample_bits()
{
  if (_rejection_sampling) {
    int k;
    if (_allow_no_mutation)
      k = _binomial_dist(Generator::engine);
    else {
      k = 0;
      while (k == 0)
        k = _binomial_dist(Generator::engine);
      assert(k > 0);
    }
    rejection_sampling(k);
  } else
    bernoulli_process();
}


void
StandardBitMutation::bernoulli_process()
{
  _flipped_bits.clear();
  bool again = true;
  do {
    for (size_t i = 0; i < _candidate.size(); i++)
      if (_bernoulli_dist(Generator::engine)) {
        _flipped_bits.push_back(i);
        again = false;
      }
  } while (again);
}


void
HammingBall::sample_bits()
{
  const int n = _candidate.size();
  const int k = _choose_k(Generator::engine);
  assert(k > 0);
  assert(k <= n);

  if (k < std::sqrt(n))
    rejection_sampling(k);
  else
    bernoulli_trials(k);
}


void
HammingSphere::sample_bits()
{
  if (_radius < std::sqrt(_candidate.size()))
    rejection_sampling(_radius);
  else
    bernoulli_trials(_radius);
}
