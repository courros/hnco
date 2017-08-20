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

#include "hnco/random.hh"

#include "walsh-expansion.hh"


using namespace hnco::random;
using namespace hnco::function;


void
WalshExpansion::random(int n, int num_features, double stddev)
{
  assert(n > 0);
  assert(num_features > 0);
  assert(stddev > 0);

  bit_vector_t bv(n);

  _features.resize(num_features);
  _coefficients.resize(num_features);
  _image.resize(num_features);

  for (size_t i = 0; i < _features.size(); i++) {
    bv_random(bv);
    _features[i] = bv;
    _coefficients[i] = stddev * Random::normal();
  }

}


double
WalshExpansion::eval(const bit_vector_t& x)
{
  bm_multiply(_features, x, _image);

  double result = 0;
  for (size_t i = 0; i < _image.size(); i++)
    if (_image[i])
      result -= _coefficients[i];
    else
      result += _coefficients[i];
  return result;
}
