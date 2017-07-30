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

#include <algorithm>            // all_of, any_of, fill

#include "neighborhood.hh"


using namespace hnco::neighborhood;
using namespace hnco::random;
using namespace hnco;


void
BernoulliProcess::sample_bits()
{
  _flipped_bits.clear();
  bool again = true;
  do {
    for (size_t i = 0; i < _candidate.size(); i++)
      if (_dist(Random::engine)) {
        _flipped_bits.push_back(i);
        again = false;
      }
  } while (again);
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
