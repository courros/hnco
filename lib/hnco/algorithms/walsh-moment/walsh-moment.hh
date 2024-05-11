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
   * Display moment.
   *
   * A TriangularMoment is displayed as a full symmetric
   * matrix with diagonal entries equal to first moments and
   * off-diagonal entries equal to second moments.
   */
  void display(std::ostream& stream);
  /// Initialize moment
  void init();
  /// Add a bit vector
  void add(const bit_vector_t& bv);
  /// Compute average
  void average(int count);
  /**
   * Update a moment.
   * @param tm Target moment
   * @param rate Learning rate
   * @post For all i, is_in_interval(first_moment[i], -1, 1)
   * @post For all j < i, is_in_interval(second_moment[i][j], -1, 1)
   *
   * This member function implements:
   *
   * self += rate * (tm1 - self)
   */
  void update(const TriangularMoment& tm, double rate);
  /**
   * Update a moment.
   * @param tm1 Target moment
   * @param tm2 Moment to move away from
   * @param rate Learning rate
   *
   * This member function implements:
   *
   * self += rate * (tm1 - tm2)
   *
   * The resulting entries are not necessarily those of a moment, that
   * is
   *
   * is_in_interval(first_moment[i], -1, 1) or
   *
   * is_in_interval(second_moment[i][j], -1, 1)
   *
   * might fail for some i, j.
   */
  void update(const TriangularMoment& tm1, const TriangularMoment& tm2, double rate);
  /**
   * Compute a scaled difference between two moments.
   * @param lambda Scale
   * @param tm1 First moment
   * @param tm2 Second moment
   *
   * This member function implements:
   *
   * self = lambda * tm1 - tm2
   *
   * It is mostly useful in herding (Hea).
   */
  void scaled_difference(double lambda, const TriangularMoment& tm1, const TriangularMoment& tm2);
  /**
   * Bound moment.
   * @param margin Distance from the -1/1 bounds
   *
   * Ensure that the distance from each moment to the -1/1 bounds is
   * greater or equal to the given margin.
   */
  void bound(double margin);
  /// 1-norm
  double norm_1() const;
  /// 2-norm
  double norm_2() const;
  /// infinite-norm
  double norm_infinite() const;
  /// distance between the moment and another moment
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
   * Display moment.
   *
   * A FullMoment is displayed as a full symmetric matrix
   * with diagonal entries equal to first moments and off-diagonal
   * entries equal to second moments.
   */
  void display(std::ostream& stream);
  /// Initialize moment
  void init();
  /// Add a bit vector
  void add(const bit_vector_t& bv);
  /**
   * Compute average.
   * @post matrix_is_symmetric(second_moment)
   */
  void average(int count);
  /**
   * Update a moment.
   * @param fm Target moment
   * @param rate Learning rate
   * @post For all i, is_in_interval(first_moment[i], -1, 1)
   * @post For all i != j, is_in_interval(second_moment[i][j], -1, 1)
   * @post matrix_is_symmetric(second_moment)
   *
   * This member function implements:
   *
   * self += rate * (fm1 - self)
   */
  void update(const FullMoment& fm, double rate);
  /**
   * Update a moment.
   * @param fm1 Target moment
   * @param fm2 Moment to move away from
   * @param rate Learning rate
   *
   * This member function implements:
   *
   * self += rate * (fm1 - fm2)
   *
   * The resulting entries are not necessarily those of a moment, that
   * is
   *
   * is_in_interval(first_moment[i], -1, 1) or
   *
   * is_in_interval(second_moment[i][j], -1, 1)
   *
   * might fail for some i != j.
   */
  void update(const FullMoment& fm1, const FullMoment& fm2, double rate);
  /**
   * Compute a scaled difference between two moments.
   * @param lambda Scale
   * @param fm1 First moment
   * @param fm2 Second moment
   *
   * This member function implements:
   *
   * self = lambda * fm1 - fm2
   *
   * It is mostly useful in herding (Hea).
   */
  void scaled_difference(double lambda, const FullMoment& fm1, const FullMoment& fm2);
  /**
   * Bound moment.
   * @param margin Distance from the -1/1 bounds
   *
   * Ensure that the distance from each moment to the -1/1 bounds is
   * greater or equal to the given margin.
   */
  void bound(double margin);
  /// 1-norm
  double norm_1() const;
  /// 2-norm
  double norm_2() const;
  /// infinite-norm
  double norm_infinite() const;
  /// distance between the moment and another moment
  double distance(const FullMoment& fm) const;
};

}
}
}

#endif
