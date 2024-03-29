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

#ifndef HNCO_ITERATOR_H
#define HNCO_ITERATOR_H

#include <assert.h>

#include "bit-vector.hh"


/// top-level HNCO namespace
namespace hnco {


/// %Iterator over bit vectors
class Iterator {

protected:

  /// Current bit vector
  bit_vector_t _current;

  /// Flag for initial state
  bool _initial_state = true;

public:

  /// Constructor
  Iterator(int n):
    _current(n) { assert(n >= 0); }

  /// Destructor
  virtual ~Iterator() {}

  /// Initialization
  virtual void init() { _initial_state = true; }

  /// Has next bit vector
  virtual bool has_next() = 0;

  /// Next bit vector
  virtual const bit_vector_t& next() = 0;

};


/** Hypercube iterator.

    Implemented as a simple binary adder.
*/
class HypercubeIterator:
    public Iterator {

public:

  /// Constructor
  HypercubeIterator(int n):
    Iterator(n) {}

  /// Has next bit vector
  bool has_next() override;

  /// Next bit vector
  const bit_vector_t& next() override;

};


/** Extended Hypercube iterator.

    Similar to Hypercube. In dimension 0, an HypercubeIterator does
    not contain any element. However, in dimension 0, an
    ExtendedHypercubeIterator contains a unique element which is the
    vector of size 0. An ExtendedHypercubeIterator is helpful when the
    enumerated vectors are seen as prefixes or suffixes hence can be
    empty. This is used, in particular, in
    compute_fast_walsh_transform.
*/
class ExtendedHypercubeIterator:
    public Iterator {

public:

  /// Constructor
  ExtendedHypercubeIterator(int n):
    Iterator(n) {}

  /// Has next bit vector
  bool has_next() override;

  /// Next bit vector
  const bit_vector_t& next() override;

};


} // end of namespace hnco


#endif
