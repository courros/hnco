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

#ifndef HNCO_NEIGHBORHOODS_NEIGHBORHOOD_ITERATOR_H
#define HNCO_NEIGHBORHOODS_NEIGHBORHOOD_ITERATOR_H

#include <assert.h>

#include "hnco/iterator.hh"


namespace hnco {

namespace neighborhood {


  /// Neighborhood iterator
  class NeighborhoodIterator:
    public Iterator {

  public:

    /** Constructor.

        \param n Size of bit vectors
    */
    NeighborhoodIterator(int n):
      Iterator(n) {}

    /// Set origin
    virtual void set_origin(const bit_vector_t& x);
  };


  /// Single bit flip neighborhood iterator
  class SingleBitFlipIterator:
    public NeighborhoodIterator {

    /// Index of the last flipped bit
    size_t _index;

  public:

    /** Constructor.

        \param n Size of bit vectors
    */
    SingleBitFlipIterator(int n):
      NeighborhoodIterator(n) {}

    /// Has next bit vector
    bool has_next();

    /// Next bit vector
    const bit_vector_t& next();

  };


  /** Hamming sphere neighborhood iterator.

      This iterator enumerates mutation masks with hamming weight
      equal to the given radius. Suppose that _mask has a first (from
      left to right) sequence of ones of length _weight and ending at
      _index:

      0 ... 0 1 ... 1 0 ...

      Then the next mask is obtained by moving to the left the first
      _weight - 1 ones and moving to the right the last one.

      1 ... 1 0 ... 0 1 ...
  */
  class HammingSphereIterator:
    public NeighborhoodIterator {

    /// Mutation mask
    bit_vector_t _mask;

    /// Radius of the ball
    int _radius;

    /// Index of the next bit to shift to the right
    int _index;

    /// Partial Hamming weight
    int _weight;

  public:

    /** Constructor.

        \param n Size of bit vectors
        \param r Radius of Hamming Ball
    */
    HammingSphereIterator(int n, int r):
      NeighborhoodIterator(n),
      _mask(n),
      _radius(r)
    {
      assert(n >= 0);
      assert(r >= 0);
      assert(r <= n);
    }

    /// Has next bit vector
    bool has_next();

    /// Next bit vector
    const bit_vector_t& next();

  };


} // end of namespace neighborhood
} // end of namespace hnco


#endif
