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

#include "util.hh"              // hnco::have_same_size
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
hnco::sbm_from_bm(sparse_bit_matrix_t& sbm, const bit_matrix_t& bm)
{
  assert(have_same_size(sbm, bm));

  for (size_t i = 0; i < sbm.size(); i++)
    sbm[i] = sbv_from_bv(bm[i]);
}

void
hnco::sbm_multiply(bit_vector_t& y, const sparse_bit_matrix_t& M, const bit_vector_t& x)
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
