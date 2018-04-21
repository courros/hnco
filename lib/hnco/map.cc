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

#include "hnco/exception.hh"

#include "map.hh"


using namespace hnco;
using namespace hnco::exception;


void
Translation::map(const bit_vector_t& input, bit_vector_t& output)
{
  assert(_bv.size() == input.size());
  assert(_bv.size() == output.size());

  for (size_t i = 0; i < output.size(); i++) {
    if (_bv[i])
      output[i] = bit_flip(input[i]);
    else
      output[i] = input[i];
  }
}


void
Permutation::map(const bit_vector_t& input, bit_vector_t& output)
{
  assert(_permutation.size() == input.size());
  assert(_permutation.size() == output.size());
  assert(perm_is_valid(_permutation));

  for (size_t i = 0; i < output.size(); i++) {
    output[i] = input[_permutation[i]];
  }
}


void
LinearMap::random(int rows, int cols, bool surjective)
{
  assert(rows > 0);
  assert(cols > 0);

  bm_resize(_bm, rows, cols);

  if (surjective) {
    if (rows > cols)
      throw Error("LinearMap::random: cols must be greater or equal to rows");
    do {
      bm_random(_bm);
    } while (!is_surjective());
  } else {
    bm_random(_bm);
  }
}


void
LinearMap::map(const bit_vector_t& input, bit_vector_t& output)
{
  assert(output.size() == _bm.size());

  bm_multiply(_bm, input, output);
}


bool
LinearMap::is_surjective()
{
  bit_matrix_t M = _bm;
  bm_row_echelon_form(M);
  return bm_rank(M) == bm_num_rows(_bm);
}


void
AffineMap::random(int rows, int cols, bool surjective)
{
  assert(rows > 0);
  assert(cols > 0);

  bm_resize(_bm, rows, cols);

  if (surjective) {
    if (rows > cols)
      throw Error("AffineMap::random: cols must be greater or equal to rows");
    do {
      bm_random(_bm);
    } while (!is_surjective());
  } else {
    bm_random(_bm);
  }

  _bv.resize(rows);
  bv_random(_bv);

  assert(bm_num_rows(_bm) == _bv.size());
}


void
AffineMap::map(const bit_vector_t& input, bit_vector_t& output)
{
  assert(output.size() == _bm.size());

  bm_multiply(_bm, input, output);
  for (size_t i = 0; i < output.size(); i++)
    if (_bv[i])
      bv_flip(output, i);
}


bool
AffineMap::is_surjective()
{
  bit_matrix_t M = _bm;
  bm_row_echelon_form(M);
  return bm_rank(M) == bm_num_rows(_bm);
}
