/* Copyright (C) 2016, 2017, 2018, 2019, 2020, 2021 Arnaud Berny

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

#ifndef HNCO_NEIGHBORHOODS_NEIGHBORHOOD_H
#define HNCO_NEIGHBORHOODS_NEIGHBORHOOD_H

#include <assert.h>

#include "hnco/bit-vector.hh"
#include "hnco/iterator.hh"
#include "hnco/random.hh"
#include "hnco/sparse-bit-vector.hh"


namespace hnco {

/** %Neighborhoods for local search.

    There are two unrelated kinds of neighborhoods, those for random
    local search and those for exhaustive local search.
*/
namespace neighborhood {


/** %Neighborhood.

    A neighborhood maintains two points, _origin and
    _candidate. They are initialized in the same state by
    set_origin. A Neighborhood class must implement the member
    function sample_bits which samples the bits to flip in _origin
    to get a _candidate. The following member functions take care of
    the modifications:

    - propose: flip _candidate
    - keep: flip _origin
    - forget flip _candidate

    After keep or forget, _origin and _candidate are in the same
    state again.

    A Neighborhood class can also behave as a mutation operator
    through the member functions mutate and map.
*/
class Neighborhood {

protected:

  /// Origin of the neighborhood
  bit_vector_t _origin;

  /// candidate bit vector
  bit_vector_t _candidate;

  /// Index distribution
  std::uniform_int_distribution<int> _index_dist;

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
    _candidate(n),
    _index_dist(0, n - 1)
  {
    assert(n > 0);
  }

  /// Destructor
  virtual ~Neighborhood() {}

  /// Set the origin
  virtual void set_origin(const bit_vector_t& x) {
    _origin = x;
    _candidate = x;
  }

  /// Get the origin
  virtual const bit_vector_t& get_origin() const { return _origin; }

  /// Get the candidate bit vector
  virtual const bit_vector_t& get_candidate() const { return _candidate; }

  /// Get flipped bits
  virtual const sparse_bit_vector_t& get_flipped_bits() const { return _flipped_bits; }

  /// Propose a candidate bit vector
  virtual void propose() {
    assert(_candidate == _origin);
    sample_bits();
    sbv_flip(_candidate, _flipped_bits);
  }

  /// Keep the candidate bit vector
  virtual void keep() {
    sbv_flip(_origin, _flipped_bits);
    assert(_candidate == _origin);
  }

  /// Forget the candidate bit vector
  virtual void forget() {
    sbv_flip(_candidate, _flipped_bits);
    assert(_candidate == _origin);
  }

  /** Mutate.

      In-place mutation of the bit vector.

      \param bv Bit vector to mutate
  */
  virtual void mutate(bit_vector_t& bv) {
    assert(bv.size() == _origin.size());
    sample_bits();
    sbv_flip(bv, _flipped_bits);
  }

  /** %Map.

      The output bit vector is a mutated version of the input bit
      vector.

      \param input Input bit vector
      \param output Output bit vector
  */
  virtual void map(const bit_vector_t& input, bit_vector_t& output) {
    assert(input.size() == _origin.size());
    assert(output.size() == _origin.size());
    copy(input.begin(), input.end(), output.begin());
    sample_bits();
    sbv_flip(output, _flipped_bits);
  }

};


/// One bit neighborhood
class SingleBitFlip:
    public Neighborhood {

  /// Sample bits
  void sample_bits() {
    assert(_flipped_bits.size() == 1);
    _flipped_bits[0] = _index_dist(random::Generator::engine);
  }

public:

  /// Constructor
  SingleBitFlip(int n):
    Neighborhood(n)
  {
    _flipped_bits.resize(1);
  }

};


/// Multi bit flip
class MultiBitFlip:
    public Neighborhood {

protected:

  /** Sample a given number of bits using Bernoulli trials.

      \param k Number of bits to sample
  */
  void bernoulli_trials(int k);

  /** Sample a given number of bits using rejection sampling.

      \param k Number of bits to sample
  */
  void rejection_sampling(int k);

public:

  /** Constructor.

      \param n Size of bit vectors
  */
  MultiBitFlip(int n):
    Neighborhood(n) {}

};


/** Standard bit mutation.

    Each component of the origin bit vector is flipped with some fixed
    probability. Unless stated otherwise, if no component has been
    flipped at the end, the process is started all over again. Thus
    the number of flipped bits follows a pseudo binomial law.

*/
class StandardBitMutation:
    public MultiBitFlip {

  /// Bernoulli distribution (biased coin)
  std::bernoulli_distribution _bernoulli_dist;

  /// Binomial distribution
  std::binomial_distribution<int> _binomial_dist;

  /// Rejection sampling
  bool _rejection_sampling = false;

  /** @name Parameters
   */
  ///@{

  /// Allow no mutation
  bool _allow_no_mutation = false;

  ///@}

  /// Sample bits
  void sample_bits();

  /// Bernoulli process
  void bernoulli_process();

public:

  /** Constructor.

      \param n Size of bit vectors

      The Bernoulli probability is set to 1 / n.
  */
  StandardBitMutation(int n):
    MultiBitFlip(n),
    _bernoulli_dist(1 / double(n)),
    _binomial_dist(n, 1 / double(n)) {}

  /** Constructor.

      \param n Size of bit vectors
      \param p Bernoulli probability
  */
  StandardBitMutation(int n, double p):
    MultiBitFlip(n),
    _bernoulli_dist(p),
    _binomial_dist(n, p) {}

  /** Set mutation rate.

      Sets _rejection_sampling to true if E(X) < sqrt(n), where X is
      a random variable with a binomial distribution B(n, p), that
      is if np < sqrt(n) or p < 1 / sqrt(n).
  */
  void set_mutation_rate(double p) {
    _bernoulli_dist = std::bernoulli_distribution(p);
    _binomial_dist = std::binomial_distribution<int>(_origin.size(), p);
    if (p < 1 / std::sqrt(_origin.size()))
      _rejection_sampling = true;
  }

  /** @name Setters
   */
  ///@{

  /// Set the flag _allow_no_mutation
  void set_allow_no_mutation(bool b) { _allow_no_mutation = b; }

  ///@}

};


/** Hamming ball.

    Choose k uniformly on [1..r], where r is the radius of the ball,
    choose k bits uniformly among n and flip them.
*/
class HammingBall:
    public MultiBitFlip {

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
    MultiBitFlip(n),
    _choose_k(1, r)
  {
    assert(r > 0);
    assert(r <= n);
  }

};


/** Hamming sphere.

    Uniformly choose r bits among n and flip them, where r is the
    radius of the sphere.
*/
class HammingSphere:
    public MultiBitFlip {

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
    MultiBitFlip(n),
    _radius(r)
  {
    assert(r > 0);
    assert(r <= n);
  }

  /// Set radius
  void set_radius(int r) { _radius = r; }

};


} // end of namespace neighborhood
} // end of namespace hnco


#endif
