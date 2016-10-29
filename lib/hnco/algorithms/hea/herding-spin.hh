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

#ifndef HNCO_ALGORITHMS_HEA_HERDING_SPIN_H
#define HNCO_ALGORITHMS_HEA_HERDING_SPIN_H

#include "moment-spin.hh"


namespace hnco {
namespace algorithm {
namespace hea {


  /** Herding with spin variables

      By spin variables, we mean variables taking values 1 or -1,
      instead of 0 or 1 in the case of binary variables.
  */
  class SpinHerding
  {

  protected:

    /// Delta moment
    SpinMoment _delta;

    /// Counter moment
    SpinMoment _count;

    /// State
    bit_vector_t _state;

    /// Permutation
    permutation_t _permutation;

    /// Choose bit
    std::uniform_int_distribution<int> _choose_bit;

    /// Time
    int _time;

    /// Compute delta
    void compute_delta(const SpinMoment& target);

    /// Sample by means of a greedy algorithm
    void sample_greedy(const SpinMoment& target, bit_vector_t& x);

    /** Function q which maximization is necessary to herding.

        Implemented for debugging purposes. */
    double q_full(const bit_vector_t& x);

    /// Derivative of q
    double q_derivative(const bit_vector_t& x, size_t i);

    /** Variation of q.

        Up to a positive multiplicative constant. Only the sign of
        the variation matters to local search. */
    double q_variation(const bit_vector_t& x, size_t i);

    /// Sample by means of random local search
    void sample_rls(const SpinMoment& target, bit_vector_t& x);

    /// Sample by means of deterministic local search
    void sample_dls(const SpinMoment& target, bit_vector_t& x);

    /// Sample by means of a neural network
    void sample_nn(const SpinMoment& target, bit_vector_t& x);

    /// Update counters
    void update_counters(const bit_vector_t& x);

  public:

    enum {
      /// Greedy algorithm
      SAMPLE_GREEDY,

      /// Random local search
      SAMPLE_RLS,

      /// Deterministic local search
      SAMPLE_DLS,

      /// Neural network
      SAMPLE_NN

    };

    /// Constructor
    SpinHerding(int n):
      _delta(n),
      _count(n),
      _state(n),
      _permutation(n, 0),
      _choose_bit(0, n - 1),
      _num_seq_updates(n),
      _num_par_updates(1) {}

    /** @name Parameters
     */
    ///@{

    /// Randomize bit order
    bool _randomize_bit_order = false;

    /// Sampling method
    int _sampling_method = SAMPLE_GREEDY;

    /// Number of sequential updates per sample
    int _num_seq_updates;

    /// Number of parallel updates per sample
    int _num_par_updates;

    /// Weight of second order moments
    double _weight = 1;

    ///@}

    /// Initialization
    void init();

    /// Compute the error
    double error(const SpinMoment& target);

    /// Compute the norm of the moment increment
    double delta(const SpinMoment& target) { return _delta.norm_2(); }

    /// Sample a bit_vector_t
    void sample(const SpinMoment& target, bit_vector_t& x);

  };


}
}
}


#endif
