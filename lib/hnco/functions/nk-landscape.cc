/* Copyright (C) 2016 Arnaud Berny

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

#include "hnco/random.hh"

#include "nk-landscape.hh"

using namespace hnco::function;
using namespace hnco::random;
using namespace std;


void
NkLandscape::random(int n, int k, double stddev)
{
  assert(n > 0);
  assert(k > 0);
  assert(stddev > 0);

  _neighbors.resize(n, vector<int>(k + 1));
  _partial_functions.resize(n, vector<double>(1 << (k + 1)));

  size_t length = _partial_functions.size();

  // Sampling k neighbors for each site
  for (size_t i = 0; i < length; i++) {
    assert(_neighbors[i].size() > 0);
    int a = _neighbors[i].size() - 1;
    int b = length - 1;
    _neighbors[i][a] = i;
    for (size_t j = 0; j < length; j++) {
      if (a == 0)
	break;
      if (j == i)
	continue;
      assert(a <= b);
      double p = double(a) / double(b);
      assert(0 <= p);
      assert(p <= 1);
      if (Random::uniform() < p) {
	a--;
	_neighbors[i][a] = j;
      }
      b--;
    }
    assert(a == 0);
  }

  // Sampling partial function values
  for (size_t i = 0; i < length; i++)
    for (size_t j = 0; j < _partial_functions[i].size(); j++)
      _partial_functions[i][j] = stddev * Random::normal();
}


double
NkLandscape::eval(const bit_vector_t& s)
{
  double result = 0;

  for (size_t i = 0; i < _partial_functions.size(); i++) {
    size_t index = 0;
    size_t base = 1;
    vector<int>& nh = _neighbors[i];
    for (size_t j = 0; j < nh.size(); j++) {
      if (s[ nh[j] ])
	index += base;
      base <<= 1; // means base *= 2;
    }
    assert(index < _partial_functions[i].size());
    result += _partial_functions[i][index];
  }

  return result / _partial_functions.size();
}
