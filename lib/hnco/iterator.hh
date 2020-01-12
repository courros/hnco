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
    bool has_next();

    /// Next bit vector
    const bit_vector_t& next();

  };


} // end of namespace hnco


#endif
