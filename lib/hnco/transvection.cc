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

#include <algorithm>            // std::shuffle
#include <math.h>               // std::sqrt

#include "random.hh"
#include "transvection.hh"


using namespace hnco;
using namespace random;


bool
Transvection::is_valid() const
{
  if (row_index < 0)
    return false;
  if (column_index < 0)
    return false;
  if (row_index == column_index)
    return false;
  return true;
}


bool
Transvection::is_valid(int n) const
{
  if (!is_valid())
    return false;
  if (row_index >= n)
    return false;
  if (column_index >= n)
    return false;
  return true;
}


void
Transvection::random(int n)
{
  assert(n > 1);

  std::uniform_int_distribution<int> index_dist(0, n - 1);
  int i, j;
  do {
    i = index_dist(Random::generator);
    j = index_dist(Random::generator);
  } while (i == j);
  row_index = i;
  column_index = j;
}


void
Transvection::random_non_commuting(int n, const Transvection& a)
{
  assert(n > 1);

  std::uniform_int_distribution<int> index_dist(0, n - 1);

  if (Random::bernoulli()) {
    row_index = a.column_index;
    int i;
    do {
      i = index_dist(Random::generator);
    } while (i == row_index);
    column_index = i;
  } else {
    column_index = a.row_index;
    int i;
    do {
      i = index_dist(Random::generator);
    } while (i == column_index);
    row_index = i;
  }

  assert(is_valid(n));
  assert(!transvections_commute(*this, a));
}


void
Transvection::multiply(bit_vector_t& x) const
{
  assert(is_valid());
  assert(is_valid(x.size()));
  assert(bv_is_valid(x));

  if (x[column_index])
    bv_flip(x, row_index);
}


void
Transvection::multiply(bit_matrix_t& M) const
{
  assert(is_valid());
  assert(is_valid(bm_num_rows(M)));

  bm_add_rows(M, column_index, row_index);
}


void
Transvection::multiply_right(bit_matrix_t& M) const
{
  assert(is_valid());
  assert(is_valid(bm_num_rows(M)));

  bm_add_columns(M, row_index, column_index);
}


bool hnco::transvections_commute(const Transvection& a, const Transvection& b)
{
  if (a.row_index == b.column_index)
    return false;
  if (b.row_index == a.column_index)
    return false;
  return true;
}


bool hnco::transvections_are_disjoint(const Transvection& a, const Transvection& b)
{
  if (a.row_index == b.row_index)
    return false;
  if (a.row_index == b.column_index)
    return false;
  if (a.column_index == b.row_index)
    return false;
  if (a.column_index == b.column_index)
    return false;
  return true;
}


bool hnco::ts_is_valid(const transvection_sequence_t& ts)
{
  return std::all_of(ts.begin(), ts.end(), [](const Transvection& tv){ return tv.is_valid(); });
}


bool hnco::ts_is_valid(const transvection_sequence_t& ts, int n)
{
  return std::all_of(ts.begin(), ts.end(), [n](const Transvection& tv){ return tv.is_valid(n); });
}


void hnco::ts_display(const transvection_sequence_t& ts, std::ostream& stream)
{
  for (auto& tv : ts) {
    tv.display(stream);
    stream << " :: ";
  }
  stream << std::endl;
}


void hnco::ts_random(transvection_sequence_t& ts, int n, int t)
{
  assert(n > 1);
  assert(t >= 0);

  ts.resize(t);
  for (auto& tv : ts)
    tv.random(n);
}


void hnco::ts_random_commuting(transvection_sequence_t& ts, int n, int t)
{
  assert(n > 1);

  if (t <= 0)
    return;

  int k = n / 2;

  int length_max;
  if (n % 2 == 0)
    length_max = k * k;
  else
    length_max = k * (k + 1);

  if (t > length_max) {
    std::cerr << "Warning: ts_random_commuting: requested sequence length too large (" << t << "), set to " << length_max << std::endl;
    t = length_max;
  }

  std::vector<int> variables(n);
  for (size_t i = 0; i < variables.size(); i++)
    variables[i] = i;
  std::shuffle(variables.begin(), variables.end(), random::Random::generator);

  double M = std::sqrt(double(n * n) / 4.0 - t);

  int split;
  if (n % 2 == 0) {
    int bound = std::min(k - 1, int(M));
    std::uniform_int_distribution<int> dist(0, bound);
    int r = dist(Random::generator);
    if (Random::bernoulli())
      split = k + r;
    else
      split = k - r;
  } else {
    int bound = std::min(k, int(M + 0.5));
    std::uniform_int_distribution<int> dist(1, bound);
    int r = dist(Random::generator);
    if (Random::bernoulli())
      split = k + r;
    else
      split = k + 1 - r;
  }

  for (int i = 0; i < split; i++) {
    for (int j = split; j < n; j++) {
      Transvection tv;
      tv.row_index = variables[i];
      tv.column_index = variables[j];
      ts.push_back(tv);
    }
  }

  std::shuffle(ts.begin(), ts.end(), random::Random::generator);

  if (t < int(ts.size()))
    ts.resize(t);
}


void hnco::ts_random_unique_source(transvection_sequence_t& ts, int n, int t)
{
  assert(n > 1);

  if (t <= 0)
    return;

  int length_max = n - 1;

  if (t > length_max) {
    std::cerr << "Warning: ts_random_unique_destination: requested sequence length too large (" << t << "), set to " << length_max << std::endl;
    t = length_max;
  }

  std::vector<int> variables(n);
  for (size_t i = 0; i < variables.size(); i++)
    variables[i] = i;
  std::shuffle(variables.begin(), variables.end(), random::Random::generator);

  std::uniform_int_distribution<int> dist(t, n - 1);

  for (int i = 0; i < t; i++) {
    Transvection tv;
    tv.row_index = variables[i];
    tv.column_index = variables[dist(Random::generator)];
    ts.push_back(tv);
  }
  assert(t == int(ts.size()));

  std::shuffle(ts.begin(), ts.end(), random::Random::generator);
}


void hnco::ts_random_unique_destination(transvection_sequence_t& ts, int n, int t)
{
  assert(n > 1);

  if (t <= 0)
    return;

  int length_max = n - 1;

  if (t > length_max) {
    std::cerr << "Warning: ts_random_unique_destination: requested sequence length too large (" << t << "), set to " << length_max << std::endl;
    t = length_max;
  }

  std::vector<int> variables(n);
  for (size_t i = 0; i < variables.size(); i++)
    variables[i] = i;
  std::shuffle(variables.begin(), variables.end(), random::Random::generator);

  std::uniform_int_distribution<int> dist(t, n - 1);

  for (int j = 0; j < t; j++) {
    Transvection tv;
    tv.row_index = variables[dist(Random::generator)];
    tv.column_index = variables[j];
    ts.push_back(tv);
  }
  assert(t == int(ts.size()));

  std::shuffle(ts.begin(), ts.end(), random::Random::generator);
}


void hnco::ts_random_disjoint(transvection_sequence_t& ts, int n, int t)
{
  assert(n > 1);

  if (t <= 0)
    return;

  std::vector<int> variables(n);
  for (size_t i = 0; i < variables.size(); i++)
    variables[i] = i;
  std::shuffle(variables.begin(), variables.end(), random::Random::generator);

  if (2 * t > n) {
    std::cerr << "Warning: ts_random_disjoint: requested sequence length too large (" << t << "), set to ";
    t = n / 2;
    std::cerr << t << std::endl;
  }
  assert(2 * t <= n);

  ts.resize(t);
  Transvection tv;
  for (size_t i = 0; i < ts.size(); i++) {
    tv.row_index = variables[2 * i];
    tv.column_index = variables[2 * i + 1];
    assert(tv.is_valid(n));
    ts[i] = tv;
  }
}


void hnco::ts_random_non_commuting(transvection_sequence_t& ts, int n, int t)
{
  assert(n > 1);

  if (t <= 0)
    return;

  ts.resize(t);
  ts[0].random(n);
  for (size_t i = 1; i < ts.size(); i++) {
    ts[i].random_non_commuting(n, ts[i - 1]);
  }
}


void hnco::ts_multiply(const transvection_sequence_t& ts, bit_vector_t& x)
{
  assert(ts_is_valid(ts));
  assert(ts_is_valid(ts, x.size()));

  for (const auto& tv : ts)
    tv.multiply(x);
}


void hnco::ts_multiply(const transvection_sequence_t& ts, bit_matrix_t& M)
{
  assert(ts_is_valid(ts));
  assert(ts_is_valid(ts, bm_num_rows(M)));

  for (const auto& tv : ts)
    tv.multiply(M);
}
