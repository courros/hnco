/* Copyright (C) 2016 Arnaud Berny

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
#include "hnco/algorithms/hea/moment-binary.hh"
#include "hnco/algorithms/matrix.hh"


namespace hnco {
namespace algorithm {
namespace hea {


  /// Herding with binary variables
  class BinaryHerding
  {

  protected:

    /// Counter moment
    BinaryMoment _count;

    /// Delta moment
    BinaryMoment _delta;

    /// Permutation
    permutation_t _permutation;

    /// Choose bit
    std::uniform_int_distribution<int> _choose_bit;

    /// Time
    int _time;

    /// Compute delta
    void compute_delta(const BinaryMoment& target);

    /// Sample a bit vector
    void sample_minimize_norm(const BinaryMoment& target, bit_vector_t& x);

    /// Sample a bit vector
    void sample_maximize_inner_product(const BinaryMoment& target, bit_vector_t& x);

  public:

    enum {
      /// Dynamics defined as minimization of a norm
      DYNAMICS_MINIMIZE_NORM,

      /// Dynamics defined as maximization of an inner product
      DYNAMICS_MAXIMIZE_INNER_PRODUCT
    };

    /// Constructor
    BinaryHerding(int n):
      _count(n),
      _delta(n),
      _permutation(n, 0),
      _choose_bit(0, n - 1) {}

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

    /// Initialization
    void init();

    /// Compute the error
    double error(const BinaryMoment& target);

    /// Compute the norm of delta
    double delta(const BinaryMoment& target) { return _delta.norm_2(); }

    /// Sample a bit vector
    void sample(const BinaryMoment& target, bit_vector_t& x);

  };

}
}
}


#endif
