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

#ifndef HNCO_ALGORITHMS_HEA_BINARY_HERDING_H
#define HNCO_ALGORITHMS_HEA_BINARY_HERDING_H

#include <vector>
#include <random>

#include "hnco/bit-vector.hh"
#include "hnco/algorithms/hea/bit-moment.hh"
#include "hnco/algorithms/matrix.hh"


namespace hnco {
namespace algorithm {
namespace hea {


  /// Herding with bit features
  class BitHerding {

  protected:

    /// Counter moment
    BitMoment _count;

    /// Delta moment
    BitMoment _delta;

    /// Permutation
    permutation_t _permutation;

    /// Choose bit
    std::uniform_int_distribution<int> _choose_bit;

    /// Time
    int _time;

    /** @name Parameters
     */
    ///@{

    /// Randomize bit order
    bool _randomize_bit_order = false;

    /// Dynamics
    int _dynamics = DYNAMICS_MINIMIZE_NORM;

    /// Weight of second order moments
    double _weight = 1;

    ///@}

    /// Compute delta
    void compute_delta(const BitMoment& target);

    /// Sample a bit vector
    void sample_minimize_norm(const BitMoment& target, bit_vector_t& x);

    /// Sample a bit vector
    void sample_maximize_inner_product(const BitMoment& target, bit_vector_t& x);

  public:

    enum {
      /// Dynamics defined as minimization of a norm
      DYNAMICS_MINIMIZE_NORM,

      /// Dynamics defined as maximization of an inner product
      DYNAMICS_MAXIMIZE_INNER_PRODUCT
    };

    /// Constructor
    BitHerding(int n):
      _count(n),
      _delta(n),
      _permutation(n, 0),
      _choose_bit(0, n - 1) {}

    /// Initialization
    void init();

    /// Sample a bit vector
    void sample(const BitMoment& target, bit_vector_t& x);

    /// Compute the error
    double error(const BitMoment& target);

    /** @name Getters
     */
    ///@{

    /// Get delta
    const BitMoment& get_delta() { return _delta; }

    ///@}

    /** @name Setters
     */
    ///@{

    /// Randomize bit order
    void set_randomize_bit_order(bool x) { _randomize_bit_order = x; }

    /// Set the dynamics
    void set_dynamics(int x) { _dynamics = x; }

    /// Set the weight of second order moments
    void set_weight(double x) { _weight = x; }

    ///@}

  };

}
}
}


#endif
