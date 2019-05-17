/* Copyright (C) 2016, 2017, 2018, 2019 Arnaud Berny

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

#include "real-representation.hh"


using namespace hnco;
using namespace hnco::function;
using namespace hnco::function::real;


DyadicRealRepresentation::DyadicRealRepresentation(double lower_bound, double upper_bound, int num_bits):
  _lower_bound(lower_bound),
  _upper_bound(upper_bound),
  _length(upper_bound - lower_bound)
{
  assert(lower_bound < upper_bound);
  assert(num_bits > 0);

  _lengths = std::vector<double>(num_bits);
  double x = 0.5;
  for (size_t i = 0; i < _lengths.size(); i++) {
    _lengths[i] = x;
    x /= 2;
  }
}


double
DyadicRealRepresentation::convert(hnco::bit_vector_t::const_iterator first, hnco::bit_vector_t::const_iterator last)
{
  double result = 0;
  for (hnco::bit_vector_t::const_iterator iter = first; iter != last; iter++) {
    if (*iter)
      result += _lengths[iter - first];
  }
  return affine_transformation(result);
}
