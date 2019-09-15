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
Transvection::random(int n, const Transvection& a)
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
    } while (i == row_index);
    row_index = i;
  }

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


bool hnco::ts_is_valid(const transvection_sequence_t& A)
{
  return std::all_of(A.begin(), A.end(), [](auto& gen){ return gen.is_valid(); });
}


bool hnco::ts_is_valid(const transvection_sequence_t& A, int n)
{
  return std::all_of(A.begin(), A.end(), [n](auto& gen){ return gen.is_valid(n); });
}


void hnco::ts_display(const transvection_sequence_t& A, std::ostream& stream)
{
  for (auto& gen : A) {
    gen.display(stream);
    stream << " :: ";
  }
  stream << std::endl;
}


void hnco::ts_random(transvection_sequence_t& A, int n, int t)
{
  assert(n > 1);
  assert(t >= 0);

  A.resize(t);
  for (auto& gen : A)
    gen.random(n);
}


void hnco::ts_random_non_commuting(transvection_sequence_t& A, int n, int t)
{
  assert(n > 1);

  if (t <= 0)
    return;

  A.resize(t);
  A[0].random(n);
  for (size_t i = 1; i < A.size(); i++) {
    A[i].random(n, A[i - 1]);
  }
}


void hnco::ts_multiply(const transvection_sequence_t& A, bit_vector_t& x)
{
  assert(ts_is_valid(A));
  assert(ts_is_valid(A, x.size()));

  for (const auto& gen : A)
    gen.multiply(x);
}


void hnco::ts_multiply(const transvection_sequence_t& A, bit_matrix_t& M)
{
  assert(ts_is_valid(A));
  assert(ts_is_valid(A, bm_num_rows(M)));

  for (const auto& gen : A)
    gen.multiply(M);
}
