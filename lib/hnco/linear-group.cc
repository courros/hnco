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
#include "linear-group.hh"


using namespace hnco;
using namespace random;


bool
Transvection::is_valid() const
{
  if (first_index < 0)
    return false;
  if (second_index < 0)
    return false;
  if (first_index == second_index)
    return false;
  return true;
}


bool
Transvection::is_valid(int n) const
{
  if (!is_valid())
    return false;
  if (first_index >= n)
    return false;
  if (second_index >= n)
    return false;
  return true;
}


void
Transvection::random(int n)
{
  assert(n > 0);

  std::uniform_int_distribution<int> index_dist(0, n - 1);
  int i, j;
  do {
    i = index_dist(Random::generator);
    j = index_dist(Random::generator);
  } while (i == j);
  first_index = i;
  second_index = j;
}


void
Transvection::apply(bit_vector_t& x) const
{
  assert(is_valid());
  assert(first_index < int(x.size()));
  assert(second_index < int(x.size()));

  x[second_index] += x[first_index];
  assert(x[second_index] == 0 ||
         x[second_index] == 1 ||
         x[second_index] == 2);
  if (x[second_index] == 2)
    x[second_index] = 0;
  return;

}


void
Transvection::apply(bit_matrix_t& M) const
{
  assert(is_valid());
  assert(bm_is_square(M));
  assert(first_index < bm_num_rows(M));
  assert(second_index < bm_num_rows(M));

  bm_add_rows(M, first_index, second_index);
}


bool hnco::ts_is_valid(const transvection_sequence_t& A)
{
  for (auto& gen : A)
    if (!gen.is_valid())
      return false;
  return true;
}


bool hnco::ts_is_valid(const transvection_sequence_t& A, int n)
{
  for (auto& gen : A)
    if (!gen.is_valid(n))
      return false;
  return true;
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
  assert(n > 0);
  assert(t > 0);

  A.resize(t);
  for (auto& gen : A)
    gen.random(n);
}


void hnco::ts_apply(const transvection_sequence_t& A, bit_vector_t& x)
{
  for (const auto& gen : A)
    gen.apply(x);
}


void hnco::ts_apply(const transvection_sequence_t& A, bit_matrix_t& M)
{
  for (const auto& gen : A)
    gen.apply(M);
}
