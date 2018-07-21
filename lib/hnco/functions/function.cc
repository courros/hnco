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

#include <math.h>               // abs

#include "hnco/iterator.hh"

#include "function.hh"


using namespace hnco::function;
using namespace hnco;


void
Function::compute_walsh_transform(std::vector<Function::WalshTransformTerm>& terms)
{
  terms.clear();

  HypercubeIterator bv_it(get_bv_size());
  HypercubeIterator features_it(get_bv_size());

  std::vector<double> coefficients(1 << get_bv_size(), 0);

  std::vector<bool> feature(get_bv_size());
  bit_vector_t bv(get_bv_size());

  bv_it.init();
  while (bv_it.has_next()) {
    const bit_vector_t& x = bv_it.next();
    double value = eval(x);

    features_it.init();
    while (features_it.has_next()) {
      const bit_vector_t& u = features_it.next();
      std::size_t index = bv_to_size_type(u);
      assert(index < coefficients.size());
      if (bv_dot_product(x, u))
        coefficients[index] -= value;
      else
        coefficients[index] += value;
    }
  }

  // Only keep non zero terms
  for (std::size_t i = 1; i < coefficients.size(); i++) {
    if (coefficients[i]) {
      bv_from_size_type(bv, i);
      bv_to_vector_bool(bv, feature);
      terms.push_back({.feature = feature, .coefficient = coefficients[i]});
    }
  }

}

bool
hnco::function::bv_is_locally_maximal(const bit_vector_t& bv, Function& fn, hnco::neighborhood::NeighborhoodIterator& it)
{
  double value = fn.eval(bv);
  it.init();
  while (it.has_next()) {
    if (fn.eval(it.next()) > value)
      return false;
  }
  return true;
}
