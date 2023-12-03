/* Copyright (C) 2016, 2017, 2018, 2019, 2020, 2021, 2022, 2023 Arnaud Berny

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

#include <algorithm>            // std::iota

#include "neighborhood-iterator.hh"


using namespace hnco::neighborhood;
using namespace hnco;


void
NeighborhoodIterator::set_origin(const bit_vector_t& x)
{
  assert(x.size() == _current.size());

  _current = x;
}

bool
SingleBitFlipIterator::has_next()
{
  if (_initial_state)
    return _current.size() > 0;
  else
    return _index + 1 < _current.size();
}


const bit_vector_t&
SingleBitFlipIterator::next()
{
  assert(has_next());

  if (_initial_state) {
    _index = 0;
    bv_flip(_current, 0);
    _initial_state = false;
  } else {
    // restore the current bit to its original value
    assert(_index < _current.size());
    bv_flip(_current, _index++);

    // Flip the next bit
    assert(_index < _current.size());
    bv_flip(_current, _index);
  }

  return _current;
}

HammingSphereIterator::HammingSphereIterator(int n, int r)
  : NeighborhoodIterator(n)
  , _radius(r)
  , _bit_indexes(r)
{
  assert(n >= 0);
  assert(r >= 0);
  assert(r <= n);
}

bool
HammingSphereIterator::has_next()
{
  if (_initial_state)
    return _current.size() > 0 && _radius > 0;

  if (_radius <= 0)
    return false;

  // Last index
  int i = _radius - 1;
  if (_bit_indexes[i] < int(_current.size()) - 1)
    return true;

  // Previous indexes
  for (i--; i >= 0; i--)
    if (_bit_indexes[i] < _bit_indexes[i + 1] - 1)
      return true;

  return false;
}

const bit_vector_t&
HammingSphereIterator::next()
{
  assert(has_next());

  if (_initial_state) {
    std::iota(_bit_indexes.begin(), _bit_indexes.end(), 0);
    _initial_state = false;
  } else {
    // Restore the current bit vector to its original value
    bv_flip(_current, _bit_indexes);

    // Last index
    int i = _radius - 1;
    if (_bit_indexes[i] < int(_current.size()) - 1) {
      _bit_indexes[i]++;
    } else {
      // Previous indexes
      for (i--; i >= 0; i--)
        if (_bit_indexes[i] < _bit_indexes[i + 1] - 1) {
          _bit_indexes[i]++;
          std::iota(_bit_indexes.begin() + i + 1, _bit_indexes.end(), _bit_indexes[i] + 1);
          break;
        }
    }
  }

  // Flip the current bit vector
  bv_flip(_current, _bit_indexes);

  return _current;
}
