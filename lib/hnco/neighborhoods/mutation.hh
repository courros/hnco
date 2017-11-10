/* Copyright (C) 2016, 2017 Arnaud Berny

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

#ifndef HNCO_NEIGHBORHOOD_MUTATION_H
#define HNCO_NEIGHBORHOOD_MUTATION_H

#include <assert.h>

#include <algorithm>            // std::copy

#include "hnco/bit-vector.hh"
#include "hnco/random.hh"


namespace hnco {
namespace neighborhood {


  /// Mutation
  class Mutation {

  protected:

  public:

    /// Destructor
    virtual ~Mutation() {}

    /// Mutate
    virtual void mutate(bit_vector_t& input) = 0;

    /// Mutate
    virtual void mutate2(const bit_vector_t& input, bit_vector_t& output) = 0;

  };


  /// Single bit flip mutation
  class SingleBitFlipMutation {

  protected:

    /// Uniform index distribution
    std::uniform_int_distribution<int> _uniform_index_dist;

  public:

    /** Constructor.

        \param n Size of bit vectors
    */
    SingleBitFlipMutation(int n):
      _uniform_index_dist(0, n - 1) {}

    /// Mutate
    void mutate(bit_vector_t& input) {
      bv_flip(input, _uniform_index_dist(random::Random::engine));
    }

    /// Mutate
    void mutate2(const bit_vector_t& input, bit_vector_t& output) {
      assert(input.size() == output.size());
      copy(input.begin(), input.end(), output.begin());
      bv_flip(output, _uniform_index_dist(random::Random::engine));
    }

  };


} // end of namespace neighborhood
} // end of namespace hnco


#endif
