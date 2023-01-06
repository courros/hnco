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

#include <algorithm>            // std::any_of

#include "iterator.hh"


using namespace hnco;


bool
HypercubeIterator::has_next()
{
  if (_initial_state)
    return _current.size() > 0;
  else
    return any_of(_current.begin(), _current.end(), [](bit_t b){ return b == 0; });
}


const bit_vector_t&
HypercubeIterator::next()
{
  assert(has_next());

  if (_initial_state) {
    bv_clear(_current);
    _initial_state = false;
  } else {
    for (size_t i = 0; i < _current.size(); i++) {
      if (_current[i])
        _current[i] = 0;
      else {
        _current[i] = 1;
        break;
      }
    }
  }

  return _current;
}


bool
ExtendedHypercubeIterator::has_next()
{
  if (_initial_state)
    return true;
  else
    return any_of(_current.begin(), _current.end(), [](bit_t b){ return b == 0; });
}


const bit_vector_t&
ExtendedHypercubeIterator::next()
{
  assert(has_next());

  if (_initial_state) {
    if (_current.size() > 0)
      bv_clear(_current);
    _initial_state = false;
  } else {
    for (size_t i = 0; i < _current.size(); i++) {
      if (_current[i])
        _current[i] = 0;
      else {
        _current[i] = 1;
        break;
      }
    }
  }

  return _current;
}
