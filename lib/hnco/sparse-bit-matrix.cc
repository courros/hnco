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

#include <assert.h>

#include "sparse-bit-matrix.hh"


using namespace hnco;


void
hnco::sbm_display(const sparse_bit_matrix_t& sbm, std::ostream& stream)
{
  for (auto sbv : sbm) {
    sbv_display(sbv, stream);
    stream << std::endl;
  }
}

void
hnco::bm_to_sbm(const bit_matrix_t& bm, sparse_bit_matrix_t& sbm)
{
  sbm = sparse_bit_matrix_t(bm.size());
  for (size_t i = 0; i < sbm.size(); i++)
    bv_to_sbv(bm[i], sbm[i]);
}

void
hnco::sbm_multiply(const sparse_bit_matrix_t& M, const bit_vector_t& x, bit_vector_t& y)
{
  assert(y.size() == M.size());

  for (size_t i = 0; i < y.size(); i++) {
    const sparse_bit_vector_t& bits = M[i];
    int sum = 0;
    for (size_t j = 0; j < bits.size(); j++)
      sum += x[bits[j]];
    y[i] = sum % 2;
  }
}
