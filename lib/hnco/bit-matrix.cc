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

#include "hnco/util.hh"         // hnco::is_in_range

#include "bit-matrix.hh"


using namespace hnco;


bit_matrix_t hnco::bm_rectangular(int nrows, int ncols)
{
  assert(nrows > 0);
  assert(ncols > 0);

  return bit_matrix_t(nrows, bit_vector_t(ncols));
}

void hnco::bm_identity(bit_matrix_t& M)
{
  assert(bm_is_valid(M));
  assert(bm_is_square(M));

  bm_clear(M);
  const int nrows = bm_num_rows(M);
  for (int i = 0; i < nrows; i++)
    M[i][i] = 1;
}

bit_matrix_t hnco::bm_identity(int n)
{
  assert(n > 0);

  bit_matrix_t result = bm_square(n);
  bm_identity(result);
  return result;
}

void hnco::bm_transpose(bit_matrix_t& N, const bit_matrix_t& M)
{
  assert(bm_is_valid(M));
  assert(bm_num_columns(N) == bm_num_rows(M));
  assert(bm_num_rows(N) == bm_num_columns(M));

  const int nrows = bm_num_rows(M);
  const int ncols = bm_num_columns(M);

  for (int i = 0; i < ncols; i++)
    for (int j = 0; j < nrows; j++)
      N[i][j] = M[j][i];
}

bit_matrix_t hnco::bm_transpose(const bit_matrix_t& M)
{
  assert(bm_is_valid(M));

  const int nrows = bm_num_rows(M);
  const int ncols = bm_num_columns(M);

  bit_matrix_t result = bm_rectangular(ncols, nrows);
  bm_transpose(result, M);
  return result;
}

void hnco::bm_display(const bit_matrix_t& M, std::ostream& stream)
{
  for (auto& bv : M) {
    bv_display(bv, stream);
    stream << std::endl;
  }
}

bool hnco::bm_is_valid(const bit_matrix_t& M)
{
  if (bm_num_rows(M) == 0)
    return false;

  const int ncols = bm_num_columns(M);

  if (ncols == 0)
    return false;

  if (!std::all_of(M.begin(), M.end(), [ncols](const bit_vector_t& row){ return int(row.size()) == ncols; }))
    return false;

  if (!std::all_of(M.begin(), M.end(), [](const bit_vector_t& row){ return bv_is_valid(row); }))
    return false;

  return true;
}

bool hnco::bm_is_identity(const bit_matrix_t& M)
{
  if (!bm_is_valid(M))
    return false;

  if (!bm_is_square(M))
    return false;

  const int nrows = bm_num_rows(M);

  for (int i = 0; i < nrows; i++) {
    auto& row = M[i];
    for (int j = 0; j < nrows; j++) {
      if (i == j) {
        if (row[j] != 1)
          return false;
      } else {
        if (row[j] != 0)
          return false;
      }
    }
  }

  return true;
}

bool hnco::bm_is_upper_triangular(const bit_matrix_t& M)
{
  const int nrows = bm_num_rows(M);
  const int ncols = bm_num_columns(M);

  for (int i = 0; i < nrows; i++)
    for (int j = 0; j < std::min(i, ncols); j++) {
      if (M[i][j] != 0)
        return false;
    }
  return true;
}

void hnco::bm_resize(bit_matrix_t& M, int nrows, int ncols)
{
  assert(nrows > 0);
  assert(ncols > 0);

  M.resize(nrows);
  for (auto& row: M)
    row.resize(ncols);
}

void hnco::bm_random(bit_matrix_t& M)
{
  for (auto& row: M)
    bv_random(row);
}

void hnco::bm_swap_rows(bit_matrix_t& M, int i, int j)
{
  assert(is_in_range(i, bm_num_rows(M)));
  assert(is_in_range(j, bm_num_rows(M)));

  const int ncols = bm_num_columns(M);

  for (int k = 0; k < ncols; k++)
    std::swap(M[i][k], M[j][k]);
}

void hnco::bm_add_rows(bit_matrix_t& M, int dest, int src)
{
  assert(is_in_range(src, bm_num_rows(M)));
  assert(is_in_range(dest, bm_num_rows(M)));
  assert(src != dest);

  bv_add(M[dest], M[src]);
}

void hnco::bm_add_columns(bit_matrix_t& M, int dest, int src)
{
  assert(is_in_range(dest, bm_num_columns(M)));
  assert(is_in_range(src, bm_num_columns(M)));
  assert(src != dest);

  const int nrows = bm_num_rows(M);

  for (int i = 0; i < nrows; i++) {
    auto& row = M[i];
    if (row[src])
      bv_flip(row, dest);
  }
}

void hnco::bm_set_column(bit_matrix_t& M, int j, const bit_vector_t& bv)
{
  assert(is_in_range(j, bm_num_columns(M)));
  assert(int(bv.size()) == bm_num_rows(M));

  const int nrows = bm_num_rows(M);

  for (int i = 0; i < nrows; i++)
    M[i][j] = bv[j];
}

void hnco::bm_row_echelon_form(bit_matrix_t& A)
{
  const int nrows = bm_num_rows(A);
  const int ncols = bm_num_columns(A);

  int r = 0;

  for (int j = 0; j < ncols; j++) {
    bool found = false;
    int pivot;
    for (int i = r; i < nrows; i++)
      if (A[i][j]) {
        pivot = i;
        found = true;
        break;
      }
    if (found) {
      if (pivot != r) {
        bm_swap_rows(A, pivot, r);
      }
      for (int i = r + 1; i < nrows; i++)
        if (A[i][j]) {
          bm_add_rows(A, i, r);
        }
      r++;
      if (r == nrows)
        break;
    }
  }
}

int hnco::bm_rank(const bit_matrix_t& A)
{
  const int nrows = bm_num_rows(A);

  int rank = 0;

  for (int i = 0; i < nrows; i++)
    if (bv_is_zero(A[i]))
      break;
    else
      rank++;

  assert(rank <= nrows);
  assert(rank <= bm_num_columns(A));

  return rank;
}

bool hnco::bm_solve(bit_matrix_t& A, bit_vector_t& b)
{
  assert(bm_is_square(A));
  assert(bm_num_rows(A) == int(b.size()));

  const int nrows = bm_num_rows(A);

  for (int i = 0; i < nrows; i++) {
    bool found = false;
    int pivot;
    for (int j = i; j < nrows; j++)
      if (A[j][i]) {
        pivot = j;
        found = true;
        break;
      }
    if (!found)
      return false;
    if (pivot != i) {
      bm_swap_rows(A, i, pivot);
      std::swap(b[i], b[pivot]);
    }
    for (int j = i + 1; j < nrows; j++)
      if (A[j][i]) {
        bm_add_rows(A, j, i);
        b[j] = (b[i] + b[j]) % 2;
      }
  }
  assert(bm_is_upper_triangular(A));

  return bm_solve_upper_triangular(A, b);
}

bool hnco::bm_solve_upper_triangular(bit_matrix_t& A, bit_vector_t& b)
{
  assert(bm_is_square(A));
  assert(bm_num_rows(A) == int(b.size()));
  assert(bm_is_upper_triangular(A));

  const int nrows = bm_num_rows(A);

  for (int k = 0; k < nrows; k++) {
    int i = nrows - 1 - k;
    for (int j = 0; j < i; j++)
      if (A[j][i]) {
        bm_add_rows(A, j, i);
        b[j] = (b[i] + b[j]) % 2;
      }
  }
  assert(bm_is_identity(A));

  return true;
}

bool hnco::bm_invert(bit_matrix_t& M, bit_matrix_t& N)
{
  assert(bm_is_square(M));
  assert(bm_is_square(N));
  assert(bm_num_rows(M) == bm_num_rows(N));

  const int nrows = bm_num_rows(M);

  bm_identity(N);
  for (int i = 0; i < nrows; i++) {
    bool found = false;
    int pivot;
    for (int j = i; j < nrows; j++)
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
    for (int j = i + 1; j < nrows; j++)
      if (M[j][i]) {
        bm_add_rows(M, j, i);
        bm_add_rows(N, j, i);
      }
  }
  assert(bm_is_upper_triangular(M));

  for (int k = 0; k < nrows; k++) {
    int i = nrows - 1 - k;
    for (int j = 0; j < i; j++)
      if (M[j][i]) {
        bm_add_rows(M, j, i);
        bm_add_rows(N, j, i);
      }
  }
  assert(bm_is_identity(M));
  return true;
}

void hnco::bm_multiply(bit_vector_t& y, const bit_matrix_t& M, const bit_vector_t& x)
{
  assert(bm_is_valid(M));
  assert(int(x.size()) == bm_num_columns(M));
  assert(int(y.size()) == bm_num_rows(M));

  for (size_t i = 0; i < y.size(); i++)
    y[i] = bv_dot_product(M[i], x);
}
