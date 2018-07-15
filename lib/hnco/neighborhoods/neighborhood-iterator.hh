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

#ifndef HNCO_NEIGHBORHOODS_NEIGHBORHOOD_ITERATOR_H
#define HNCO_NEIGHBORHOODS_NEIGHBORHOOD_ITERATOR_H

#include <assert.h>

#include "hnco/bit-vector.hh"
#include "hnco/iterator.hh"
#include "hnco/random.hh"
#include "hnco/sparse-bit-vector.hh"


namespace hnco {

namespace neighborhood {


  /// Neighborhood iterator
  class NeighborhoodIterator:
    public Iterator {

  protected:

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

    /// Initialization
    void init();

    /// Has next bit vector
    bool has_next();

    /// Next bit vector
    void next();

  };


  /// Hamming sphere neighborhood iterator
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
      assert(n > 0);
      assert(r > 0);
      assert(r <= n);
    }

    /// Initialization
    void init();

    /// Has next bit vector
    bool has_next();

    /// Next bit vector
    void next();

  };


} // end of namespace neighborhood
} // end of namespace hnco


#endif
