/* Copyright (C) 2016, 2017, 2018, 2019, 2020, 2021, 2022, 2023, 2024 Arnaud Berny

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

#include <sstream>              // std::ostringstream

#include "hnco/exception.hh"

#include "map.hh"


using namespace hnco::exception;
using namespace hnco::map;
using namespace hnco;


void
Translation::map(const bit_vector_t& input, bit_vector_t& output)
{
  assert(input.size() == _bv.size());
  assert(output.size() == _bv.size());

  bv_add(output, input, _bv);
}


void
Translation::display(std::ostream& stream) const
{
  const int length = 5;
  stream << "Translation(" << _bv.size() << "): ";
  int bound = std::min(length, int(_bv.size()));
  for (int i = 0; i < bound; i++)
    if (_bv[i])
      stream << 1;
    else
      stream << 0;
  if (length < int(_bv.size()))
    stream << "...";
  stream << std::endl;
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
      throw std::runtime_error("LinearMap::random: cols must be greater or equal to rows");
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

  bm_multiply(output, _bm, input);
}


bool
LinearMap::is_surjective() const
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
      throw std::runtime_error("AffineMap::random: cols must be greater or equal to rows");
    do {
      bm_random(_bm);
    } while (!is_surjective());
  } else {
    bm_random(_bm);
  }

  _bv.resize(rows);
  bv_random(_bv);

  assert(bm_num_rows(_bm) == int(_bv.size()));
}


void
AffineMap::map(const bit_vector_t& input, bit_vector_t& output)
{
  assert(output.size() == _bm.size());

  bm_multiply(output, _bm, input);
  bv_add(output, _bv);
}


bool
AffineMap::is_surjective() const
{
  bit_matrix_t M = _bm;
  bm_row_echelon_form(M);
  return bm_rank(M) == bm_num_rows(_bm);
}


void
AffineMap::display(std::ostream& stream) const
{
  stream << "AffineMap: " << get_input_size() << " bits -> " << get_output_size() << " bits" << std::endl;
  stream << "Bit matrix is:" << std::endl;
  bm_display(_bm, stream);
  stream << "Bit vector is:" << std::endl;
  bv_display(_bv, stream);
  stream << std::endl;
}


Injection::Injection(const std::vector<int>& bit_positions, int output_size):
  _bit_positions(bit_positions),
  _output_size(output_size)
{
  if (output_size < int(bit_positions.size()))
    throw std::runtime_error("Injection::Injection: output size must be greater or equal to input size");
}


void
Injection::map(const bit_vector_t& input, bit_vector_t& output)
{
  assert(input.size() == _bit_positions.size());
  assert(int(output.size()) == _output_size);

  for (std::size_t i = 0; i < _bit_positions.size(); i++)
    output[_bit_positions[i]] = input[i];
}


Projection::Projection(const std::vector<int>& bit_positions, int input_size):
  _bit_positions(bit_positions),
  _input_size(input_size)
{
  if (input_size < int(bit_positions.size()))
    throw std::runtime_error("Projection::Projection: input size must be greater or equal to output size");
}


void
Projection::map(const bit_vector_t& input, bit_vector_t& output)
{
  assert(int(input.size()) == _input_size);
  assert(output.size() == _bit_positions.size());

  for (std::size_t i = 0; i < _bit_positions.size(); i++)
    output[i] = input[_bit_positions[i]];
}


void
TsAffineMap::random(int n, int t, SamplingMode mode)
{
  assert(n > 0);
  assert(t > 0);

  _bv.resize(n);
  bv_random(_bv);

  switch (mode) {

  case SamplingMode::unconstrained:
    ts_random(_ts, n, t);
    break;

  case SamplingMode::commuting_transvections:
    ts_random_commuting(_ts, n, t);
    break;

  case SamplingMode::unique_source:
    ts_random_unique_source(_ts, n, t);
    break;

  case SamplingMode::unique_destination:
    ts_random_unique_destination(_ts, n, t);
    break;

  case SamplingMode::disjoint_transvections:
    ts_random_disjoint(_ts, n, t);
    break;

  case SamplingMode::non_commuting_transvections:
    ts_random_non_commuting(_ts, n, t);
    break;

  default:
    throw std::runtime_error("TsAffineMap::random: Unknown sampling mode: " + std::to_string(static_cast<int>(mode)));
  }
}


void
TsAffineMap::map(const bit_vector_t& input, bit_vector_t& output)
{
  assert(input.size() == _bv.size());
  assert(output.size() == _bv.size());

  output = input;
  ts_multiply(output, _ts);
  bv_add(output, _bv);
}


void
TsAffineMap::display(std::ostream& stream) const
{
  ts_display(_ts, stream);
}
