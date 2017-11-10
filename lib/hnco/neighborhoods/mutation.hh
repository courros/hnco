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

  public:

    /// Destructor
    virtual ~Mutation() {}

    /** Mutate.

        In-place mutation of the bit vector.

        \param bv Bit vector to mutate
    */
    virtual void mutate(bit_vector_t& bv) = 0;

    /** Map.

        The output bit vector is a mutated version of the input bit
        vector.

        \param input Input bit vector
        \param output Output bit vector
    */
    virtual void map(const bit_vector_t& input, bit_vector_t& output) = 0;

  };


  /// Single bit flip mutation
  class SingleBitFlipMutation:
    public Mutation {

    /// Bit vector size
    size_t _bv_size;

    /// Uniform index distribution
    std::uniform_int_distribution<int> _uniform_index_dist;

  public:

    /** Constructor.

        \param n Size of bit vectors
    */
    SingleBitFlipMutation(int n):
      _bv_size(n),
      _uniform_index_dist(0, n - 1) {}

    void mutate(bit_vector_t& bv) {
      assert(bv.size() == _bv_size);
      bv_flip(bv, _uniform_index_dist(random::Random::engine));
    }

    void map(const bit_vector_t& input, bit_vector_t& output) {
      assert(input.size() == _bv_size);
      assert(input.size() == output.size());
      copy(input.begin(), input.end(), output.begin());
      bv_flip(output, _uniform_index_dist(random::Random::engine));
    }

  };


  /** Bernoulli process mutation.

      Each component of the bit vector is flipped with some fixed
      probability. If no component has been flipped at the end, the
      process is started all over again. Thus the number of flipped
      bits follows a pseudo binomial law.

  */
  class BernoulliProcessMutation:
    public Mutation {

    /// Bit vector size
    size_t _bv_size;

    /// Bernoulli distribution (biased coin)
    std::bernoulli_distribution _bernoulli_dist;

    /// Binomial distribution
    std::binomial_distribution<int> _binomial_dist;

    /// Reservoir sampling
    bool _reservoir_sampling = false;

  public:

    /** Constructor.

        \param n Size of bit vectors
    */
    BernoulliProcessMutation(int n):
      _bv_size(n) {}

    void mutate(bit_vector_t& bv) {
      assert(bv.size() == _bv_size);

    }

    void map(const bit_vector_t& input, bit_vector_t& output) {
      assert(input.size() == _bv_size);
      assert(input.size() == output.size());

    }

  };


} // end of namespace neighborhood
} // end of namespace hnco


#endif
