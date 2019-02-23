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

#include <algorithm>            // fill

#include "hnco/exception.hh"

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

bool
HammingSphereIterator::has_next()
{
  if (_initial_state)
    return _current.size() > 0 && _radius > 0;

  assert(bv_is_valid(_mask));
  assert(bv_hamming_weight(_mask) == _radius);

  _weight = 0;
  for (size_t i = 0; i < _mask.size() - 1; i++)
    if (_mask[i] == 1) {
      _weight++;
      if (_mask[i + 1] == 0) {
        _index = i;
        return true;
      }
    }
  return false;
}

const bit_vector_t&
HammingSphereIterator::next()
{
  assert(has_next());

  if (_initial_state) {
    bv_clear(_mask);
    fill(_mask.begin(), _mask.begin() + _radius, 1);
    assert(bv_hamming_weight(_mask) == _radius);
    bv_flip(_current, _mask);
    _initial_state = false;
  } else {
    // restore the current bit vector to its original value
    bv_flip(_current, _mask);

    // update mask
    assert(_mask[_index] == 1);
    assert(_mask[_index + 1] == 0);

    _mask[_index + 1] = 1;
    fill(_mask.begin(), _mask.begin() + _index + 1, 0);
    fill(_mask.begin(), _mask.begin() + _weight - 1, 1);

    assert(bv_is_valid(_mask));
    assert(bv_hamming_weight(_mask) == _radius);

    // Flip the current bit vector
    bv_flip(_current, _mask);
  }

  return _current;
}
