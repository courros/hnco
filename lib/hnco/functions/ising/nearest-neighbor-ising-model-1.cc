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

#include <assert.h>

#include "hnco/random.hh"

#include "nearest-neighbor-ising-model-1.hh"


using namespace hnco::random;
using namespace hnco::function;


void
NearestNeighborIsingModel1::resize(int n)
{
  assert(n > 0);

  _coupling.resize(n);
  _field.resize(n);
  _flipped_bits = bit_vector_t(n, 0);
}


double
NearestNeighborIsingModel1::eval(const bit_vector_t& s)
{
  assert(_coupling.size() > 0);
  assert(s.size() == _coupling.size());

  const size_t n = _coupling.size();
  const size_t last = n - 1;

  double result = 0;

  // Interactions
  for (size_t i = 0; i < last; i++)
    if ((s[i] + s[i + 1]) % 2 == 0)
      result += _coupling[i];
    else
      result -= _coupling[i];
  if (_periodic_boundary_conditions) {
    if ((s[last] + s[0]) % 2 == 0)
      result += _coupling[last];
    else
      result -= _coupling[last];
  }

  // External field
  for (size_t i = 0; i < n; i++)
    if (s[i])
      result -= _field[i];
    else
      result += _field[i];

  return result;
}


double
NearestNeighborIsingModel1::incremental_eval(const bit_vector_t& x,
                                             double value,
                                             const hnco::sparse_bit_vector_t& flipped_bits)
{
  assert(_coupling.size() > 0);
  assert(x.size() == _coupling.size());

  const size_t n = _coupling.size();
  const size_t last = n - 1;

  assert(bv_is_zero(_flipped_bits));
  bv_flip(_flipped_bits, flipped_bits);

  // Interactions with sites on the right
  for (auto index : flipped_bits) {
    size_t neighbor;
    if (index == last) {
      if (_periodic_boundary_conditions)
        neighbor = 0;
      else
        continue;
    } else {
      neighbor = index + 1;
    }
    assert(neighbor < n);
    if (_flipped_bits[neighbor])
      continue;
    if ((x[index] + x[neighbor]) % 2 == 0)
      value -= 2 * _coupling[index];
    else
      value += 2 * _coupling[index];
  }

  // Interactions with sites on the left
  for (auto index : flipped_bits) {
    size_t neighbor;
    if (index == 0) {
      if (_periodic_boundary_conditions)
        neighbor = last;
      else
        continue;
    } else {
      assert(index > 0);
      neighbor = index - 1;
    }
    assert(neighbor < n);
    if (_flipped_bits[neighbor])
      continue;
    if ((x[neighbor] + x[index]) % 2 == 0)
      value -= 2 * _coupling[neighbor];
    else
      value += 2 * _coupling[neighbor];
  }

  bv_flip(_flipped_bits, flipped_bits);
  assert(bv_is_zero(_flipped_bits));

  // External field
  for (auto index : flipped_bits) {
    if (x[index])
      value += 2 * _field[index];
    else
      value -= 2 * _field[index];
  }

  return value;
}


void
NearestNeighborIsingModel1::display(std::ostream& stream)
{
  stream << "NearestNeighborIsingModel1" << std::endl;
  stream << "n = " << get_bv_size() << std::endl;
  stream << "Periodic boundary condition = " << _periodic_boundary_conditions << std::endl;
}
