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

#ifndef HNCO_ALGORITHMS_LS_NEIGHBORHOOD_H
#define HNCO_ALGORITHMS_LS_NEIGHBORHOOD_H

#include <assert.h>

#include "hnco/bit-vector.hh"
#include "hnco/iterator.hh"
#include "hnco/random.hh"
#include "hnco/sparse-bit-vector.hh"


namespace hnco {

/** Neighborhoods for local search.

    There are two unrelated kinds of neighborhoods, those for random
    local search and those for exhaustive local search.
*/
namespace neighborhood {

  /** Neighborhood.

      A neighborhood maintains two points, _origin and
      _candidate. They are initialized in the same state by
      set_origin. The neighborhood must implement the method
      sample_bits which samples the bits to flip in _origin to get a
      _candidate. The following methods take care of the
      modifications:

      - propose: flip _candidate
      - keep: flip _origin
      - forget flip _candidate

      After keep or forget, _origin and _candidate are in the same
      state again.
   */
  class Neighborhood {

  protected:

    /// Origin of the neighborhood
    bit_vector_t _origin;

    /// candidate bit vector
    bit_vector_t _candidate;

    /// Flipped bits
    sparse_bit_vector_t _flipped_bits;

    /// Sample bits
    virtual void sample_bits() = 0;

  public:

    /** Constructor.

        \param n Size of bit vectors
    */
    Neighborhood(int n):
      _origin(n),
      _candidate(n) {}

    /// Destructor
    virtual ~Neighborhood() {}

    /// Set the origin
    virtual void set_origin(const bit_vector_t& x) {
      _origin = x;
      _candidate = x;
    }

    /// Get the origin
    virtual const bit_vector_t& get_origin() { return _origin; }

    /// Get the candidate bit vector
    virtual const bit_vector_t& get_candidate() { return _candidate; }

    /// Get flipped bits
    virtual const sparse_bit_vector_t& get_flipped_bits() { return _flipped_bits; }

    /// Propose a candidate bit vector
    virtual void propose() {
      assert(_candidate == _origin);
      sample_bits();
      bv_flip(_candidate, _flipped_bits);
    }

    /// Keep the candidate bit vector
    virtual void keep() {
      bv_flip(_origin, _flipped_bits);
      assert(_candidate == _origin);
    }

    /// Forget the candidate bit vector
    virtual void forget() {
      bv_flip(_candidate, _flipped_bits);
      assert(_candidate == _origin);
    }

  };


  /// One bit neighborhood
  class SingleBitFlip:
    public Neighborhood {

    /// Uniform index distribution
    std::uniform_int_distribution<int> _uniform_index_dist;

    /// Sample bits
    void sample_bits() {
      assert(_flipped_bits.size() == 1);
      _flipped_bits[0] = _uniform_index_dist(random::Random::engine);;
    }

  public:

    /// Constructor
    SingleBitFlip(int n):
      Neighborhood(n),
      _uniform_index_dist(0, n - 1)
    {
      assert(n > 0);
      _flipped_bits.resize(1);
    }

  };


  /** Bernoulli process.

      Each component of the origin bit vector is flipped with some
      fixed probability. If no component has been flipped at the end,
      the process is started all over again. Thus the number of
      flipped bits follows a pseudo binomial law.

  */
  class BernoulliProcess:
    public Neighborhood {

    /// Bernoulli distribution (biased coin)
    std::bernoulli_distribution _bernoulli_dist;

    /// Binomial distribution
    std::binomial_distribution<int> _binomial_dist;

    /// Uniform index distribution
    std::uniform_int_distribution<int> _uniform_index_dist;

    /// Reservoir sampling
    bool _reservoir_sampling = false;

    /// Bernoulli process
    void do_bernoulli_process();

    /// Reservoir sampling
    void do_reservoir_sampling();

    /// Sample bits
    void sample_bits() {
      if (_reservoir_sampling)
        do_reservoir_sampling();
      else
        do_bernoulli_process();
    }

  public:

    /** Constructor.

        \param n Size of bit vectors

        The Bernoulli probability is set to 1 / n.
    */
    BernoulliProcess(int n):
      Neighborhood(n),
      _bernoulli_dist(1 / double(n)),
      _binomial_dist(n, 1 / double(n)),
      _uniform_index_dist(0, n - 1) {}

    /** Constructor.

        \param n Size of bit vectors
        \param p Bernoulli probability
    */
    BernoulliProcess(int n, double p):
      Neighborhood(n),
      _bernoulli_dist(p),
      _binomial_dist(n, p),
      _uniform_index_dist(0, n - 1) {}

    /** Set probability.

        Sets _reservoir_sampling to true if E(X) < sqrt(n), where X is
        a random variable with a binomial distribution B(n, p), that
        is if np < sqrt(n) or p < 1 / sqrt(n).
    */
    void set_probability(double p) {
      _bernoulli_dist = std::bernoulli_distribution(p);
      _binomial_dist = std::binomial_distribution<int>(_origin.size(), p);
      if (p < 1 / std::sqrt(_origin.size()))
        _reservoir_sampling = true;
    }

  };


  /** Hamming ball.

      Choose k uniformly on [1..r], where r is the radius of the ball,
      choose k bits uniformly among n and flip them.

  */
  class HammingBall:
    public Neighborhood {

    /// Radius of the ball
    int _radius;

    /// Choose the distance to the center
    std::uniform_int_distribution<int> _choose_k;

    /// Sample bits
    void sample_bits();

  public:

    /** Constructor.

        \param n Size of bit vectors
        \param r Radius of the ball
    */
    HammingBall(int n, int r):
      Neighborhood(n),
      _radius(r),
      _choose_k(1, r)
    {
      assert(n > 0);
      assert(r > 0);
      assert(r <= n);
    }

  };


  /** Hamming sphere.

      Uniformly choose r bits among n and flip them, where r is the
      radius of the sphere.
  */
  class HammingSphere:
    public Neighborhood {

    /// Radius of the sphere
    int _radius;

    /// Sample bits
    void sample_bits();

  public:

    /** Constructor.

        \param n Size of bit vectors
        \param r Radius of the sphere
    */
    HammingSphere(int n, int r):
      Neighborhood(n),
      _radius(r)
    {
      assert(n > 0);
      assert(r > 0);
      assert(r <= n);
    }

  };


} // end of namespace neighborhood
} // end of namespace hnco


#endif
