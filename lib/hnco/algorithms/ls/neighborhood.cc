/* Copyright (C) 2016, 2017 Arnaud Berny

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

#include <algorithm>            // find

#include "neighborhood.hh"


using namespace hnco::neighborhood;
using namespace hnco::random;
using namespace hnco;


void
BernoulliProcess::do_bernoulli_process()
{
  _flipped_bits.clear();
  bool again = true;
  do {
    for (size_t i = 0; i < _candidate.size(); i++)
      if (_bernoulli_dist(Random::engine)) {
        _flipped_bits.push_back(i);
        again = false;
      }
  } while (again);
}


void
BernoulliProcess::do_reservoir_sampling()
{
  int k = 0;
  while (k == 0)
    k = _binomial_dist(Random::engine);
  assert(k > 0);

  _flipped_bits.clear();
  for (int i = 0; i < k; i++) {
    int index;
    do {
      index = _uniform_index_dist(Random::engine);
    } while (find(begin(_flipped_bits),
                  end(_flipped_bits),
                  index) != _flipped_bits.end());
    _flipped_bits.push_back(index);
  }
}


void
HammingBall::sample_bits()
{
  int n = _candidate.size();
  int k = _choose_k(Random::engine);
  assert(k <= n);

  _flipped_bits.clear();
  for (size_t i = 0; i < _candidate.size(); i++) {
    double p = double(k) / double(n);
    if (Random::uniform() < p) {
      _flipped_bits.push_back(i);
      k--;
    }
    n--;
  }

  assert(k == 0);
}


void
HammingSphere::sample_bits()
{
  int n = _candidate.size();
  int k = _radius;
  assert(k <= n);

  _flipped_bits.clear();
  for (size_t i = 0; i < _candidate.size(); i++) {
    double p = double(k) / double(n);
    if (Random::uniform() < p) {
      _flipped_bits.push_back(i);
      k--;
    }
    n--;
  }

  assert(k == 0);
}
