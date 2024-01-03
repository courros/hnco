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

#ifndef HNCO_NEIGHBORHOODS_NEIGHBORHOOD_ITERATOR_H
#define HNCO_NEIGHBORHOODS_NEIGHBORHOOD_ITERATOR_H

#include "hnco/iterator.hh"
#include "hnco/sparse-bit-vector.hh"


namespace hnco {
namespace neighborhood {


/** %Neighborhood iterator.

    A neighborhood iterator allows to iterate over bit vectors in the
    neighborhood of a given origin. The origin itself should not
    belong to the neighborhood.
*/
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
  bool has_next() override;

  /// Next bit vector
  const bit_vector_t& next() override;

};


/** Hamming sphere neighborhood iterator.

    The Hamming sphere iterator is implemented using an array of
    indexes which indicate the bits to flip in the given origin.

    For example, in dimension n = 4 and with radius = 2, the sequence
    of indexes is as follows (assuming indexes start at 1):
    - 12 (first state, bits 1 and 2 are flipped)
    - 13
    - 14
    - 23 (last index cannot be increased, first index is increased and second index is reset)
    - 24
    - 34

    Reference: https://en.wikipedia.org/wiki/Combination#Enumerating_k-combinations
*/
class HammingSphereIterator:
    public NeighborhoodIterator {

  /// Radius of the ball
  int _radius;

  /// Bit indexes
  sparse_bit_vector_t _bit_indexes;

public:

  /** Constructor.

      \param n Size of bit vectors
      \param r Radius of Hamming Ball
  */
  HammingSphereIterator(int n, int r);

  /// Has next bit vector
  bool has_next() override;

  /// Next bit vector
  const bit_vector_t& next() override;

};


} // end of namespace neighborhood
} // end of namespace hnco


#endif
