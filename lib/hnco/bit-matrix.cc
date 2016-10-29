/* Copyright (C) 2016 Arnaud Berny

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

#include <assert.h>		// assert

#include "bit-matrix.hh"
#include "exception.hh"


using namespace std;
using namespace hnco;
using namespace hnco::exception;

void hnco::bm_identity(bit_matrix_t& M)
{
  for (size_t i = 0; i < M.size(); i++)
    for (size_t j = 0; j < M.size(); j++) {
      if (i == j)
        M[i][j] = 1;
      else
        M[i][j] = 0;
    }
}

bool hnco::bm_is_identity(const bit_matrix_t& M)
{
  for (size_t i = 0; i < M.size(); i++)
    for (size_t j = 0; j < M.size(); j++) {
      if (i == j) {
        if (M[i][j] != 1)
          return false;
      } else {
        if (M[i][j] != 0)
          return false;
      }
    }
  return true;
}

void hnco::bm_resize(bit_matrix_t& M, int num_rows, int num_columns)
{
  assert(num_rows > 0);
  assert(num_columns > 0);

  M.resize(num_rows, bit_vector_t(num_columns));
}

void hnco::bm_random(bit_matrix_t& M)
{
  for (auto& bv: M)
    bv_random(bv);
}

void hnco::bm_swap_rows(bit_matrix_t& M, size_t i, size_t j)
{
  assert(i < M.size());
  assert(j < M.size());
  for (size_t k = 0; k < M.size(); k++) {
    int tmp = M[i][k];
    M[i][k] = M[j][k];
    M[j][k] = tmp;
  }
}

void hnco::bm_add_rows(bit_matrix_t& M, size_t i, size_t j)
{
  assert(i < M.size());
  assert(j < M.size());
  assert(i != j);
  bv_add(M[i], M[j]);
}

bool hnco::bm_invert(bit_matrix_t& M, bit_matrix_t& N)
{
  bm_identity(N);
  for (size_t i = 0; i < M.size(); i++) {
    bool found = false;
    size_t pivot;
    for (size_t j = i; j < M.size(); j++)
      if (M[j][i]) {
        pivot = j;
        found = true;
        break;
      }
    if (!found)
      return false;
    if (pivot != i) {
      bm_swap_rows(M, i, pivot);
      bm_swap_rows(N, i, pivot);
    }
    for (size_t j = i + 1; j < M.size(); j++)
      if (M[j][i]) {
        bm_add_rows(M, i, j);
        bm_add_rows(N, i, j);
      }
  }
  // assert: M is upper triangular
  for (size_t k = 0; k < M.size(); k++) {
    size_t i = M.size() - 1 - k;
    for (size_t j = 0; j < i; j++)
      if (M[j][i]) {
        bm_add_rows(M, i, j);
        bm_add_rows(N, i, j);
      }
  }
  assert(bm_is_identity(M));
  return true;
}

void hnco::bm_multiply(const bit_matrix_t& M, const bit_vector_t& x, bit_vector_t& y)
{
  assert(bm_is_valid(M));
  assert(x.size() == bm_num_columns(M));
  assert(y.size() == bm_num_rows(M));

  for (size_t i = 0; i < y.size(); i++)
    y[i] = bv_dot_product(M[i], x);
}

void hnco::bm_transpose(const bit_matrix_t& M, bit_matrix_t& N)
{
  assert(bm_is_valid(M));

  size_t rows = bm_num_rows(M);
  size_t cols = bm_num_columns(M);

  bm_resize(N, cols, rows);
  for (size_t i = 0; i < cols; i++)
    for (size_t j = 0; j < rows; j++)
      N[i][j] = M[j][i];
}
