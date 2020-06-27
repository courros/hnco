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
#include <math.h>		// log

#include "hnco/random.hh"

#include "probability-vector.hh"

using namespace hnco::random;


double hnco::algorithm::pv_entropy(const pv_t& pv)
{
  double s = 0;
  for (size_t i = 0; i < pv.size(); i++) {
    double p = pv[i];
    if (p == 0 ||
	p == 1)
      continue;
    s += p * log(p) + (1 - p) * log(1 - p);
  }
  return -s;
}

void hnco::algorithm::pv_sample(bit_vector_t& x, const pv_t& pv)
{
  assert(x.size() == pv.size());

  for (size_t i = 0; i < x.size(); i++) {
    if (Generator::uniform() < pv[i])
      x[i] = 1;
    else
      x[i] = 0;
  }
}

void hnco::algorithm::pv_add(pv_t& pv, const bit_vector_t& x)
{
  assert(x.size() == pv.size());

  for (size_t i = 0; i < pv.size(); i++)
    if (x[i] == 1)
      pv[i]++;
}

void hnco::algorithm::pv_add(pv_t& pv, const bit_vector_t& x, double weight)
{
  assert(x.size() == pv.size());

  for (size_t i = 0; i < pv.size(); i++)
    if (x[i] == 1)
      pv[i] += weight;
}

void hnco::algorithm::pv_average(pv_t& pv, int count)
{
  for (size_t i = 0; i < pv.size(); i++)
    pv[i] /= count;
}

void hnco::algorithm::pv_update(pv_t& pv, double rate, const bit_vector_t& x)
{
  assert(x.size() == pv.size());

  for (size_t i = 0; i < pv.size(); i++)
    pv[i] += rate * (x[i] - pv[i]);
}

void hnco::algorithm::pv_update(pv_t& pv, double rate, const std::vector<double>& x)
{
  assert(x.size() == pv.size());

  for (size_t i = 0; i < pv.size(); i++)
    pv[i] += rate * (x[i] - pv[i]);
}

void hnco::algorithm::pv_update(pv_t& pv, double rate, const std::vector<double>& x, const std::vector<double>& y)
{
  assert(x.size() == pv.size());
  assert(y.size() == pv.size());

  for (size_t i = 0; i < pv.size(); i++)
    pv[i] += rate * (x[i] - y[i]);
}

void hnco::algorithm::pv_bound(pv_t& pv, double lower_bound, double upper_bound)
{
  for (size_t i = 0; i < pv.size(); i++) {
    pv[i] = std::min(upper_bound, pv[i]);
    pv[i] = std::max(lower_bound, pv[i]);
  }
}
