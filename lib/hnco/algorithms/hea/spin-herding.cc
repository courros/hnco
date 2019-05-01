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

#include <math.h>		// sqrt

#include "hnco/algorithms/matrix.hh"
#include "hnco/exception.hh"
#include "hnco/permutation.hh"
#include "hnco/util.hh"

#include "spin-herding.hh"


using namespace hnco::algorithm::hea;
using namespace hnco::exception;
using namespace hnco::random;


void
SpinHerding::init()
{
  _time = 0;
  _count.init();
  perm_identity(_permutation);
}


double
SpinHerding::error(const SpinMoment& target)
{
  double result = 0;

  const size_t dimension = _count._first.size();
  for (size_t i = 0; i < dimension; i++) {
    result += square(_time * target._first[i] - _count._first[i]);
    double tmp = 0;
    for (size_t j = 0; j < i; j++)
      tmp += square(_time * target._second[i][j] - _count._second[i][j]);
    result += _weight * tmp;
  }
  return sqrt(result) / (_time * target.diameter());
}


void
SpinHerding::sample(const SpinMoment& target, bit_vector_t& x)
{
  compute_delta(target);

  switch (_sampling_method) {
  case SAMPLE_GREEDY:
    sample_greedy(x);
    break;
  case SAMPLE_RLS:
    sample_rls(x);
    break;
  case SAMPLE_DLS:
    sample_dls(x);
    break;
  default:
    std::ostringstream stream;
    stream << _sampling_method;
    throw Error("SpinHerding::sample: Unknown _sampling_method: " + stream.str());
  }

  _count.add(x);
}


void
SpinHerding::compute_delta(const SpinMoment& target)
{
  _time++;

  const size_t dimension = _count._first.size();

  for (size_t i = 0; i < dimension; i++) {
    _delta._first[i] = _time * target._first[i] - _count._first[i];

    std::vector<double>& line = _delta._second[i];
    const std::vector<double>& line2 = target._second[i];
    const std::vector<double>& line3 = _count._second[i];
    for (size_t j = 0; j < i; j++)
      line[j] = _time * line2[j] - line3[j];
  }

  assert(matrix_is_strictly_lower_triangular(_delta._second));
}


void
SpinHerding::sample_greedy(bit_vector_t& x)
{
  if (_randomize_bit_order)
    perm_random(_permutation);

  for (int k = 0; k < int(_permutation.size()); k++) {
    int i = _randomize_bit_order ? _permutation[k] : k;

    double acc = 0;
    for (int l = 0; l < k; l++) {
      int j = _randomize_bit_order ? _permutation[l] : l;
      if (j < i) {
        if (x[j])
          acc -= _delta._second[i][j];
        else
          acc += _delta._second[i][j];
      } else {
        if (x[j])
          acc -= _delta._second[j][i];
        else
          acc += _delta._second[j][i];
      }
    }

    if (_delta._first[i] + _weight * acc > 0)
      x[i] = 0;
    else
      x[i] = 1;
  }
}


double
SpinHerding::q_derivative(const bit_vector_t& x, int i)
{
  assert(i < int(_delta._first.size()));

  double result = _delta._first[i];
  for (int j = 0; j < int(_delta._second.size()); j++) {
    if (j == i)
      continue;
    if (x[j])
      result += _delta._second[i][j];
    else
      result -= _delta._second[i][j];
  }
  return result;
}


double
SpinHerding::q_variation(const bit_vector_t& x, int i)
{
  assert(i < int(_delta._first.size()));

  if (x[i])
    return -q_derivative(x, i);   // transition 1 -> 0
  else
    return q_derivative(x, i);    // transition 0 -> 1
}


void
SpinHerding::sample_rls(bit_vector_t& x)
{
  bv_random(x);

  int failures = 0;
  for (int t = 0; t < _num_seq_updates; t++) {
    int i = _choose_bit(Random::generator);
    if (q_variation(x, i) >= 0) {
      bv_flip(x, i);
      failures = 0;
    } else
      failures++;
  }
}


void
SpinHerding::sample_dls(bit_vector_t& x)
{
  bv_random(x);

  if (_randomize_bit_order)
    perm_random(_permutation);

  int failures = 0;
  for (int t = 0; t < _num_seq_updates; t++) {
    int index = t % int(x.size());
    int i = _randomize_bit_order ? _permutation[index] : index;
    if (q_variation(x, i) >= 0) {
      bv_flip(x, i);
      failures = 0;
    } else
      failures++;
    if (failures == int(x.size()))
      break;
  }
}
