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

#ifndef HNCO_ALGORITHMS_WALSH_MOMENT_WALSH_MOMENT_H
#define HNCO_ALGORITHMS_WALSH_MOMENT_WALSH_MOMENT_H

#include <iosfwd>

#include "hnco/bit-vector.hh"

namespace hnco {
namespace algorithm {
namespace walsh_moment {

/**
 * Triangular moment.
 */
struct TriangularMoment {
  /// First moment
  std::vector<double> first_moment;
  /// Second moment
  std::vector<std::vector<double>> second_moment;
  /**
   * Constructor.
   * @param n Size of bit vector
   */
  TriangularMoment(int n);
  /**
   * Display Walsh moment.
   *
   * A TriangularMoment is displayed as a full symmetric
   * matrix with diagonal entries equal to first moments and
   * off-diagonal entries equal to second moments.
   */
  void display(std::ostream& stream);
  /// Initialize Walsh moment
  void init();
  /// Add a bit vector to a Walsh moment
  void add(const bit_vector_t& bv);
  /// Average each Walsh moment
  void average(int count);
  /**
   * Update a Walsh moment.
   * @param wm Target Walsh moment
   * @param rate Learning rate
   * @post For all i, is_in_interval(first_moment[i], -1, 1)
   * @post For all j < i, is_in_interval(second_moment[i][j], -1, 1)
   *
   * This member function implements:
   *
   * self += rate * (wm1 - self)
   */
  void update(const TriangularMoment& wm, double rate);
  /**
   * Update a Walsh moment.
   * @param wm1 Target Walsh moment
   * @param wm2 Walsh moment to move away from
   * @param rate Learning rate
   *
   * This member function implements:
   *
   * self += rate * (wm1 - wm2)
   *
   * The resulting entries are not necessarily those of a Walsh
   * moment, that is
   *
   * is_in_interval(first_moment[i], -1, 1) or
   *
   * is_in_interval(second_moment[i][j], -1, 1)
   *
   * might fail for some i, j.
   */
  void update(const TriangularMoment& wm1, const TriangularMoment& wm2, double rate);
  /**
   * Compute a scaled difference between two moments.
   * @param lambda Scale
   * @param wm1 First Walsh moment
   * @param wm2 Second Walsh moment
   *
   * This member function implements:
   *
   * self = lambda * wm1 - wm2
   *
   * It is mostly useful in herding (Hea).
   */
  void scaled_difference(double lambda, const TriangularMoment& wm1, const TriangularMoment& wm2);
  /**
   * Bound Walsh moment.
   * @param margin Distance from the -1/1 bounds
   *
   * Ensure that the distance from each Walsh moment to the -1/1
   * bounds is greater or equal to the given margin.
   */
  void bound(double margin);
  /// 1-norm of the Walsh moment
  double norm_1() const;
  /// 2-norm of the Walsh moment
  double norm_2() const;
  /// infinite-norm of the Walsh moment
  double norm_infinite() const;
  /// distance between the Walsh moment and another Walsh moment
  double distance(const TriangularMoment& wm) const;
};

/**
 * Full moment.
 */
struct FullMoment {
  /// First moment
  std::vector<double> first_moment;
  /// Second moment
  std::vector<std::vector<double>> second_moment;
  /**
   * Constructor.
   * @param n Size of bit vector
   */
  FullMoment(int n);
  /**
   * Display Walsh moment.
   *
   * A FullMoment is displayed as a full symmetric matrix
   * with diagonal entries equal to first moments and off-diagonal
   * entries equal to second moments.
   */
  void display(std::ostream& stream);
  /// Initialize Walsh moment
  void init();
  /// Add a bit vector to a Walsh moment
  void add(const bit_vector_t& bv);
  /**
   * Average each Walsh moment.
   * @post matrix_is_symmetric(second_moment)
   */
  void average(int count);
  /**
   * Update a Walsh moment.
   * @param wm Target Walsh moment
   * @param rate Learning rate
   * @post For all i, is_in_interval(first_moment[i], -1, 1)
   * @post For all i != j, is_in_interval(second_moment[i][j], -1, 1)
   * @post matrix_is_symmetric(second_moment)
   *
   * This member function implements:
   *
   * self += rate * (wm1 - self)
   */
  void update(const FullMoment& wm, double rate);
  /**
   * Update a Walsh moment.
   * @param wm1 Target Walsh moment
   * @param wm2 Walsh moment to move away from
   * @param rate Learning rate
   *
   * This member function implements:
   *
   * self += rate * (wm1 - wm2)
   *
   * The resulting entries are not necessarily those of a Walsh moment, that is
   *
   * is_in_interval(first_moment[i], -1, 1) or
   *
   * is_in_interval(second_moment[i][j], -1, 1)
   *
   * might fail for some i != j.
   */
  void update(const FullMoment& wm1, const FullMoment& wm2, double rate);
  /**
   * Compute a scaled difference between two moments.
   * @param lambda Scale
   * @param wm1 First Walsh moment
   * @param wm2 Second Walsh moment
   *
   * This member function implements:
   *
   * self = lambda * wm1 - wm2
   *
   * It is mostly useful in herding (Hea).
   */
  void scaled_difference(double lambda, const FullMoment& wm1, const FullMoment& wm2);
  /**
   * Bound Walsh moment.
   * @param margin Distance from the -1/1 bounds
   *
   * Ensure that the distance from each Walsh moment to the -1/1
   * bounds is greater or equal to the given margin.
   */
  void bound(double margin);
  /// 1-norm of the Walsh moment
  double norm_1() const;
  /// 2-norm of the Walsh moment
  double norm_2() const;
  /// infinite-norm of the Walsh moment
  double norm_infinite() const;
  /// distance between the Walsh moment and another Walsh moment
  double distance(const FullMoment& wm) const;
};

}
}
}

#endif
