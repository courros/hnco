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


// Static member
const std::string hnco::GlGenerator::names[] = { "Permutation", "Transvection" };


bool
GlGenerator::is_valid() const
{
  if (type != Type::Permutation &&
      type != Type::Transvection)
    return false;
  if (first_index < 0)
    return false;
  if (second_index < 0)
    return false;
  return true;
}


void
GlGenerator::random(int n)
{
  assert(n > 0);

  if (Random::bernoulli())
    type = Type::Permutation;
  else
    type = Type::Transvection;

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
GlGenerator::apply(bit_vector_t& x) const
{
  assert(is_valid());
  assert(first_index < int(x.size()));
  assert(second_index < int(x.size()));

  if (type == Type::Permutation) {
    std::swap(x[first_index], x[second_index]);
    return;
  }

  if (type == Type::Transvection) {
    x[second_index] += x[first_index];
    assert(x[second_index] == 0 ||
           x[second_index] == 1 ||
           x[second_index] == 2);
    if (x[second_index] == 2)
      x[second_index] = 0;
    return;
  }

}


void
GlGenerator::apply(bit_matrix_t& M) const
{
  assert(is_valid());
  assert(bm_is_square(M));
  assert(first_index < bm_num_rows(M));
  assert(second_index < bm_num_rows(M));

  if (type == Type::Permutation) {
    bm_swap_rows(M, first_index, second_index);
    return;
  }

  if (type == Type::Transvection) {
    bm_add_rows(M, first_index, second_index);
    return;
  }

}


void hnco::gl_display(const gl_element_t& A, std::ostream& stream)
{
  for (auto& gen : A) {
    gen.display(stream);
    stream << " :: ";
  }
  stream << std::endl;
}


void hnco::gl_random(gl_element_t& A, int n, int t)
{
  assert(n > 0);
  assert(t > 0);

  A = gl_element_t(t);
  for (auto& gen : A)
    gen.random(n);
}


void hnco::gl_apply(const gl_element_t& A, bit_vector_t& x)
{
  for (const auto& gen : A)
    gen.apply(x);
}


void hnco::gl_apply(const gl_element_t& A, bit_matrix_t& M)
{
  for (const auto& gen : A)
    gen.apply(M);
}
