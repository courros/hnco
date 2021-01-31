/* Copyright (C) 2016, 2017, 2018, 2019, 2020, 2021 Arnaud Berny

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

#include "hnco/random.hh"

#include "nearest-neighbor-ising-model-2.hh"


using namespace hnco::random;
using namespace hnco::function;


void
NearestNeighborIsingModel2::resize(int num_rows, int num_columns)
{
  assert(num_rows > 0);
  assert(num_columns > 0);

  _coupling_right = std::vector<std::vector<double> >(num_rows, std::vector<double>(num_columns));
  _coupling_below = std::vector<std::vector<double> >(num_rows, std::vector<double>(num_columns));
  _field = std::vector<std::vector<double> >(num_rows, std::vector<double>(num_columns));

  _flipped_bits = bit_vector_t(get_bv_size(), 0);
}


double
NearestNeighborIsingModel2::evaluate(const bit_vector_t& s)
{
  assert(_field.size() > 0);
  assert(_field[0].size() > 0);
  assert(int(s.size()) == get_bv_size());

  const int num_rows = _field.size();
  const int last_row = num_rows - 1;

  const int num_columns = _field[0].size();
  const int last_column = num_columns - 1;

  double result = 0;

  int start;

  // Right
  start = 0;
  for (int i = 0; i < num_rows; i++, start += num_columns) {
    for (int j = 0; j < last_column; j++) {
      int site = start + j;
      assert(site + 1 < int(s.size()));
      if ((s[site] + s[site + 1]) % 2 == 0)
        result += _coupling_right[i][j];
      else
        result -= _coupling_right[i][j];
    }
  }

  // Below
  start = 0;
  for (int i = 0; i < last_row; i++, start += num_columns) {
    for (int j = 0; j < num_columns; j++) {
      int site = start + j;
      assert(site + num_columns < int(s.size()));
      if ((s[site] + s[site + num_columns]) % 2 == 0)
        result += _coupling_below[i][j];
      else
        result -= _coupling_below[i][j];
    }
  }

  if (_periodic_boundary_conditions) {

    // Last column is connected to the first column
    start = 0;
    for (int i = 0; i < num_rows; i++, start += num_columns) {
      // site:  s[start + last_column]
      // right: s[start]
      if ((s[start + last_column] + s[start]) % 2 == 0)
        result += _coupling_right[i][last_column];
      else
        result -= _coupling_right[i][last_column];
    }

    // Last row is connected to the first row
    start = last_row * num_columns;
    for (int j = 0; j < num_columns; j++) {
      // site:  s[start + j]
      // below: s[j]
      if ((s[start + j] + s[j]) % 2 == 0)
        result += _coupling_below[last_row][j];
      else
        result -= _coupling_below[last_row][j];
    }

  }

  // External field
  start = 0;
  for (int i = 0; i < num_rows; i++, start += num_columns) {
    for (int j = 0; j < num_columns; j++) {
      int site = start + j;
      if (s[site])
        result -= _field[i][j];
      else
        result += _field[i][j];
    }
  }

  return result;
}


double
NearestNeighborIsingModel2::evaluate_incrementally(const bit_vector_t& x,
                                                   double value,
                                                   const hnco::sparse_bit_vector_t& flipped_bits)
{
  assert(_field.size() > 0);
  assert(_field[0].size() > 0);
  assert(int(x.size()) == get_bv_size());

  const int num_rows = _field.size();
  const int last_row = num_rows - 1;

  const int num_columns = _field[0].size();
  const int last_column = num_columns - 1;

  const int n = get_bv_size();

  assert(bv_is_zero(_flipped_bits));
  sbv_flip(_flipped_bits, flipped_bits);

  // Interactions with sites on the right
  for (auto index : flipped_bits) {
    int i = index / num_columns;
    int j = index % num_columns;
    int neighbor;
    if (j == last_column) {
      if (_periodic_boundary_conditions) {
        assert(last_column <= index);
        neighbor = index - last_column;
      } else
        continue;
    } else
      neighbor = index + 1;
    assert(index < n);
    assert(neighbor < n);
    if (_flipped_bits[neighbor])
      continue;
    if ((x[index] + x[neighbor]) % 2 == 0)
      value -= 2 * _coupling_right[i][j];
    else
      value += 2 * _coupling_right[i][j];
  }

  // Interactions with sites on the left
  for (auto index : flipped_bits) {
    int i = index / num_columns;
    int j = index % num_columns;
    int neighbor;
    if (j == 0) {
      if (_periodic_boundary_conditions) {
        neighbor = index + last_column;
        j = last_column;
      } else
        continue;
    } else {
      assert(index > 0);
      neighbor = index - 1;
      j--;
    }
    assert(index < n);
    assert(neighbor < n);
    assert(j < num_columns);
    if (_flipped_bits[neighbor])
      continue;
    if ((x[index] + x[neighbor]) % 2 == 0)
      value -= 2 * _coupling_right[i][j];
    else
      value += 2 * _coupling_right[i][j];
  }

  // Interactions with sites below
  for (auto index : flipped_bits) {
    int i = index / num_columns;
    int j = index % num_columns;
    int neighbor;
    if (i == last_row) {
      if (_periodic_boundary_conditions)
        neighbor = j;
      else
        continue;
    } else
      neighbor = index + num_columns;
    assert(index < n);
    assert(neighbor < n);
    if (_flipped_bits[neighbor])
      continue;
    if ((x[index] + x[neighbor]) % 2 == 0)
      value -= 2 * _coupling_below[i][j];
    else
      value += 2 * _coupling_below[i][j];
  }

  // Interactions with sites above
  for (auto index : flipped_bits) {
    int i = index / num_columns;
    int j = index % num_columns;
    int neighbor;
    if (i == 0) {
      if (_periodic_boundary_conditions) {
        neighbor = (n + j) - num_columns;
        i = last_row;
      }
      else
        continue;
    } else {
      assert(num_columns <= index);
      neighbor = index - num_columns;
      i--;
    }
    assert(index < n);
    assert(neighbor < n);
    assert(i < num_rows);
    if (_flipped_bits[neighbor])
      continue;
    if ((x[index] + x[neighbor]) % 2 == 0)
      value -= 2 * _coupling_below[i][j];
    else
      value += 2 * _coupling_below[i][j];
  }

  sbv_flip(_flipped_bits, flipped_bits);
  assert(bv_is_zero(_flipped_bits));

  // External field
  for (auto index : flipped_bits) {
    int i = index / num_columns;
    int j = index % num_columns;
    if (x[index])
      value += 2 * _field[i][j];
    else
      value -= 2 * _field[i][j];
  }

  return value;
}


void
NearestNeighborIsingModel2::display(std::ostream& stream)
{
  assert(_field.size() > 0);

  stream << "NearestNeighborIsingModel2" << std::endl;
  stream << "nrows = " << _field.size() << std::endl;
  stream << "ncols = " << _field[0].size() << std::endl;
  stream << "bv_size = " << get_bv_size() << std::endl;
  stream << "Periodic boundary condition = " << _periodic_boundary_conditions << std::endl;
}
