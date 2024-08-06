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

#ifndef HNCO_ALGORITHMS_WALSH_MOMENT_HERDING_H
#define HNCO_ALGORITHMS_WALSH_MOMENT_HERDING_H

#include "hnco/permutation.hh"

#include "walsh-moment.hh"

/**
 * @file herding.hh
 * Herding algorihms.
 *
 * Herding algorithms for the generation of quasi-random sequences of
 * bit vectors of given second Walsh moment.
 */

namespace hnco {
namespace algorithm {
namespace walsh_moment {

/**
 * Herding with lower triangular Walsh moment.
 */
class TriangularMomentHerding {
  /// Delta moment
  TriangularMoment _delta;
  /// Counter moment
  TriangularMoment _count;
  /// Error moment
  TriangularMoment _error;
  /// Permutation
  permutation_t _permutation;
  /// Time
  int _time;
  /**
   * @name Parameters
   */
  ///@{
  /// Randomize bit order
  bool _randomize_bit_order = true;
  ///@}

public:
  /// Walsh moment type
  using Moment = TriangularMoment;
  /**
   * Constructor.
   * @param n Size of bit vectors
   */
  TriangularMomentHerding(int n):
    _delta(n),
    _count(n),
    _error(n),
    _permutation(n, 0) {}
  /// Initialization
  void init();
  /// Sample a bit vector
  void sample(const TriangularMoment& target, bit_vector_t& x);
  /// Compute the error
  double error(const TriangularMoment& target);
  /**
   * @name Getters
   */
  ///@{
  /// Get delta
  const TriangularMoment& get_delta() const { return _delta; }
  ///@}
  /**
   * @name Setters
   */
  ///@{
  /// Randomize bit order
  void set_randomize_bit_order(bool b) { _randomize_bit_order = b; }
  ///@}
};

/**
 * Herding with full moments.
 */
class FullMomentHerding {
  /// Delta moment
  FullMoment _delta;
  /// Counter moment
  FullMoment _count;
  /// Error moment
  FullMoment _error;
  /// Permutation
  permutation_t _permutation;
  /// Time
  int _time;
  /**
   * @name Parameters
   */
  ///@{
  /// Randomize bit order
  bool _randomize_bit_order = true;
  ///@}

public:
  /// Walsh moment type
  using Moment = FullMoment;
  /**
   * Constructor.
   * @param n Size of bit vectors
   */
  FullMomentHerding(int n):
    _delta(n),
    _count(n),
    _error(n),
    _permutation(n, 0) {}
  /// Initialization
  void init();
  /// Sample a bit vector
  void sample(const FullMoment& target, bit_vector_t& x);
  /// Compute the error
  double error(const FullMoment& target);
  /**
   * @name Getters
   */
  ///@{
  /// Get delta
  const FullMoment& get_delta() const { return _delta; }
  ///@}
  /**
   * @name Setters
   */
  ///@{
  /// Randomize bit order
  void set_randomize_bit_order(bool b) { _randomize_bit_order = b; }
  ///@}
};

}
}
}

#endif
