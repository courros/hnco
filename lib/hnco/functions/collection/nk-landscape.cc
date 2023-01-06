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

#include "hnco/random.hh"

#include "nk-landscape.hh"

using namespace hnco::function;
using namespace hnco::random;


void
NkLandscape::random_structure(int n, int k)
{
  assert(n > 0);
  assert(k > 0);

  _neighbors.resize(n, std::vector<int>(k + 1));
  _partial_functions.resize(n, std::vector<double>(1 << (k + 1)));

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
      if (Generator::uniform() < p) {
	a--;
	_neighbors[i][a] = j;
      }
      b--;
    }
    assert(a == 0);
  }
}


double
NkLandscape::evaluate(const bit_vector_t& s)
{
  double result = 0;

  for (size_t i = 0; i < _partial_functions.size(); i++) {
    size_t index = 0;
    size_t base = 1;
    std::vector<int>& nh = _neighbors[i];
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


void
NkLandscape::display(std::ostream& stream) const
{
  stream << "NK landscape with N = " << get_bv_size()
         << " and K = " << (_neighbors[0].size() - 1) << std::endl;
  stream << "Neighbors:" << std::endl;
  for (size_t i = 0; i < _neighbors.size(); i++) {
    stream << i << " -> ";
    auto ns = _neighbors[i];
    for (auto n : ns)
      stream << n << " ";
    stream << std::endl;
  }
}
