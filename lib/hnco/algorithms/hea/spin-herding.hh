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

#ifndef HNCO_ALGORITHMS_HEA_SPIN_HERDING_H
#define HNCO_ALGORITHMS_HEA_SPIN_HERDING_H

#include "hnco/permutation.hh"

#include "spin-moment.hh"


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
  LowerTriangularWalshMoment2 _delta;

  /// Counter moment
  LowerTriangularWalshMoment2 _count;

  /// Error moment
  LowerTriangularWalshMoment2 _error;

  /// Permutation
  permutation_t _permutation;

  /// Time
  int _time;

  /** @name Parameters
   */
  ///@{

  /// Randomize bit order
  bool _randomize_bit_order = false;

  ///@}

public:

  /** Constructor.

      \param n Size of bit vectors
  */
  SpinHerding(int n):
    _delta(n),
    _count(n),
    _error(n),
    _permutation(n, 0) {}

  /// Initialization
  void init();

  /// Sample a bit vector
  void sample(const LowerTriangularWalshMoment2& target, bit_vector_t& x);

  /// Compute the error
  double error(const LowerTriangularWalshMoment2& target);

  /** @name Getters
   */
  ///@{

  /// Get delta
  const LowerTriangularWalshMoment2& get_delta() const { return _delta; }

  ///@}

  /** @name Setters
   */
  ///@{

  /// Randomize bit order
  void set_randomize_bit_order(bool x) { _randomize_bit_order = x; }

  ///@}

};


}
}
}


#endif
