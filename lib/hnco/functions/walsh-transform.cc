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

#include <math.h>

#include "hnco/iterator.hh"

#include "walsh-transform.hh"


using namespace hnco::function;
using namespace hnco;


void
WalshTransform::compute()
{
  HypercubeIterator bv_it(_function->get_bv_size());
  HypercubeIterator features_it(_function->get_bv_size());

  features_it.init();
  while (true) {
    const bit_vector_t& u = features_it.get_current();
    std::size_t index = 0;
    std::size_t base = 1;
    for (size_t i = 0; i < u.size(); i++) {
      if (u[i])
	index += base;
      base <<= 1;               // means base *= 2;
    }
    assert(index < _coefficients.size());

    double sum = 0.0;
    bv_it.init();
    while (true) {
      double value = _function->eval(bv_it.get_current());
      if (bv_dot_product(bv_it.get_current(), features_it.get_current()))
        sum -= value;
      else
        sum += value;
      if (bv_it.has_next()) {
        bv_it.next();
      } else {
        break;                  // Exit the innermost loop
      }
    }
    _coefficients[index] = sum / std::pow(2, u.size() / 2.0);

    if (features_it.has_next()) {
      features_it.next();
    } else {
      break;
    }
  }

  _coefficients[0] = 0;
}
