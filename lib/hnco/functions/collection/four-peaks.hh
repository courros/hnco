/* Copyright (C) 2016, 2017, 2018, 2019, 2020, 2021, 2022 Arnaud Berny

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

#ifndef HNCO_FUNCTIONS_COLLECTION_FOUR_PEAKS_H
#define HNCO_FUNCTIONS_COLLECTION_FOUR_PEAKS_H

#include "hnco/functions/function.hh"


namespace hnco {
namespace function {


/** Four Peaks.

    It is defined by

    f(x) = max{head(x, 1) + tail(x, 0)} + R(x)

    where:
    - head(x, 1) is the length of the longest prefix of x made of ones;
    - tail(x, 0) is the length of the longest suffix of x made of zeros;
    - R(x) is the reward;
    - R(x) = n if (head(x, 1) > t and tail(x, 0) > t);
    - R(x) = 0 otherwise;
    - the threshold t is a parameter of the function.

    This function has four maxima, of which exactly two are global
    ones.

    For example, if n = 6 and t = 1:
    - f(111111) = 6 (local maximum)
    - f(111110) = 5
    - f(111100) = 10 (global maximum)

    Reference:

    S. Baluja and R. Caruana. 1995. Removing the genetics from the
    standard genetic algorithm. In Proceedings of the 12th Annual
    Conference on Machine Learning.  38–46.

*/
class FourPeaks:
    public Function {

  /// Bit vector size
  int _bv_size;

  /// Threshold
  int _threshold;

  /// Maximum
  int _maximum;

public:

  /// Constructor
  FourPeaks(int bv_size, int threshold);

  /// Get bit vector size
  int get_bv_size() const override { return _bv_size; }

  /** Check for a known maximum.
   *
   * \return true
   */
  bool has_known_maximum() const override { return true; }

  /** Get the global maximum.
      \return 2 * _bv_size - _threshold - 1 */
  double get_maximum() const override { return _maximum; }

  /// Evaluate a bit vector
  double evaluate(const bit_vector_t&) override;

};


/** Six Peaks.

    It is defined by

    f(x) = max{head(x, 0) + tail(x, 1) + head(x, 1) + tail(x, 0)} + R(x)

    where:
    - head(x, 0) is the length of the longest prefix of x made of zeros;
    - head(x, 1) is the length of the longest prefix of x made of ones;
    - tail(x, 0) is the length of the longest suffix of x made of zeros;
    - tail(x, 1) is the length of the longest suffix of x made of ones;
    - R(x) is the reward;
    - R(x) = n if (head(x, 0) > t and tail(x, 1) > t) or (head(x, 1) > t and tail(x, 0) > t);
    - R(x) = 0 otherwise;
    - the threshold t is a parameter of the function.

    This function has six maxima, of which exactly four are global
    ones.

    For example, if n = 6 and t = 1:
    - f(111111) = 6 (local maximum)
    - f(111110) = 5
    - f(111100) = 10 (global maximum)

    Reference:

    J. S. De Bonet, C. L. Isbell, and P. Viola. 1996. MIMIC: finding
    optima by estimating probability densities. In Advances in
    Neural Information Processing Systems. Vol. 9.  MIT Press,
    Denver.

*/
class SixPeaks:
    public Function {

  /// Bit vector size
  int _bv_size;

  /// Threshold
  int _threshold;

  /// Maximum
  int _maximum;

public:

  /// Constructor
  SixPeaks(int bv_size, int threshold);

  /// Get bit vector size
  int get_bv_size() const override { return _bv_size; }

  /** Check for a known maximum.
   *
   * \return true
   */
  bool has_known_maximum() const override { return true; }

  /** Get the global maximum.
      \return 2 * _bv_size - _threshold - 1 */
  double get_maximum() const override { return _maximum; }

  /// Evaluate a bit vector
  double evaluate(const bit_vector_t&) override;

};


} // end of namespace function
} // end of namespace hnco


#endif
