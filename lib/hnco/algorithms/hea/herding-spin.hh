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
  class SpinHerding {

  protected:

    /// Delta moment
    SpinMoment _delta;

    /// Counter moment
    SpinMoment _count;

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

    /// Sampling method
    int _sampling_method = SAMPLE_GREEDY;

    /// Number of sequential updates per sample
    int _num_seq_updates;

    /// Weight of second order moments
    double _weight = 1;

    ///@}

    /// Compute delta
    void compute_delta(const SpinMoment& target);

    /// Sample by means of a greedy algorithm
    void sample_greedy(bit_vector_t& x);

    /// Derivative of q
    double q_derivative(const bit_vector_t& x, size_t i);

    /** Variation of q.

        Up to a positive multiplicative constant. Only the sign of
        the variation matters to local search. */
    double q_variation(const bit_vector_t& x, size_t i);

    /// Sample by means of random local search
    void sample_rls(bit_vector_t& x);

    /// Sample by means of deterministic local search
    void sample_dls(bit_vector_t& x);

  public:

    enum {
      /// Greedy algorithm
      SAMPLE_GREEDY,

      /// Random local search
      SAMPLE_RLS,

      /// Deterministic local search
      SAMPLE_DLS,

      LAST_SAMPLE
    };

    /** Constructor.

        \param n Size of bit vectors

        _num_seq_updates is initialized to n.
    */
    SpinHerding(int n):
      _delta(n),
      _count(n),
      _permutation(n, 0),
      _choose_bit(0, n - 1),
      _num_seq_updates(n) {}

    /// Initialization
    void init();

    /// Sample a bit vector
    void sample(const SpinMoment& target, bit_vector_t& x);

    /// Compute the error
    double error(const SpinMoment& target);

    /** @name Getters
     */
    ///@{

    /// Get delta
    const SpinMoment& get_delta() { return _delta; }

    ///@}

    /** @name Setters
     */
    ///@{

    /// Randomize bit order
    void set_randomize_bit_order(bool x) { _randomize_bit_order = x; }

    /// Set the sampling method
    void set_sampling_method(int x) { _sampling_method = x; }

    /// Set the number of sequential updates per sample
    void set_num_seq_updates(int x) { _num_seq_updates = x; }

    /// Set the weight of second order moments
    void set_weight(double x) { _weight = x; }

    ///@}

  };


}
}
}


#endif
