/* Copyright (C) 2016, 2017, 2018, 2019, 2020, 2021, 2022 Arnaud Berny

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

#include <algorithm>            // std::copy, std::copy_backward

#include "hnco/iterator.hh"

#include "function.hh"


using namespace hnco::function;
using namespace hnco;


void
hnco::function::compute_walsh_transform(function::Function *fn, std::vector<function::WalshTerm>& terms)
{
  const int n = fn->get_bv_size();

  std::vector<double> coefficients(1 << n, 0.0);

  HypercubeIterator bv_it(n);
  HypercubeIterator features_it(n);

  bv_it.init();
  while (bv_it.has_next()) {
    const bit_vector_t& x = bv_it.next();
    double value = fn->evaluate(x);

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

  terms.clear();

  std::vector<bool> feature(n);
  bit_vector_t bv(n);

  for (std::size_t i = 0; i < coefficients.size(); i++) {
    if (coefficients[i]) {
      bv_from_size_type(bv, i);
      bv_to_vector_bool(feature, bv);
      terms.push_back({.feature = feature, .coefficient = coefficients[i]});
    }
  }

}

void
hnco::function::compute_fast_walsh_transform(function::Function *fn, std::vector<function::WalshTerm>& terms)
{
  const int n = fn->get_bv_size();

  std::vector<double> coefficients(1 << n, 0.0);
  bit_vector_t x(n);

  for (int k = 0; k < n; k++) {
    ExtendedHypercubeIterator prefixes(k);
    while (prefixes.has_next()) {
      const auto& u = prefixes.next();
      std::copy(u.begin(), u.end(), x.begin());
      ExtendedHypercubeIterator suffixes(n - k - 1);
      while (suffixes.has_next()) {
        const auto& v = suffixes.next();
        std::copy_backward(v.begin(), v.end(), x.end());
        x[k] = 0;
        size_t index0 = bv_to_size_type(x);
        assert(index0 < coefficients.size());
        x[k] = 1;
        size_t index1 = bv_to_size_type(x);
        assert(index1 < coefficients.size());
        double f0, f1;
        if (k == 0) {
          x[k] = 0;
          f0 = fn->evaluate(x);
          x[k] = 1;
          f1 = fn->evaluate(x);
        } else {
          f0 = coefficients[index0];
          f1 = coefficients[index1];
        }
        coefficients[index0] = f0 + f1;
        coefficients[index1] = f0 - f1;
      }
    }
  }

  // Only keep non zero terms

  terms.clear();

  std::vector<bool> feature(n);
  bit_vector_t bv(n);

  for (std::size_t i = 0; i < coefficients.size(); i++) {
    if (coefficients[i]) {
      bv_from_size_type(bv, i);
      bv_to_vector_bool(feature, bv);
      terms.push_back({.feature = feature, .coefficient = coefficients[i]});
    }
  }

}

bool
hnco::function::bv_is_locally_maximal(const bit_vector_t& bv, Function& fn, hnco::neighborhood::NeighborhoodIterator& it)
{
  double value = fn.evaluate(bv);
  it.set_origin(bv);
  it.init();
  while (it.has_next()) {
    if (fn.evaluate(it.next()) > value)
      return false;
  }
  return true;
}

bool
hnco::function::bv_is_globally_maximal(const bit_vector_t& bv, Function& fn)
{
  assert(fn.has_known_maximum());
  return fn.evaluate(bv) == fn.get_maximum();
}
