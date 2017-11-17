/* Copyright (C) 2016, 2017 Arnaud Berny

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

#ifndef HNCO_FUNCTIONS_FOUR_PEAKS_H
#define HNCO_FUNCTIONS_FOUR_PEAKS_H

#include "function.hh"


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

      De Bonet, J. S., Isbell, C. L., & Viola, P. (1997). MIMIC:
      Finding optima by estimating probability densities. Advances in
      neural information processing systems, 424-430.
  */
  class FourPeaks:
    public Function {

    /// Bit vector size
    size_t _bv_size;

    /// Threshold
    int _threshold;

    /// Maximum
    int _maximum;

  public:

    /// Constructor
    FourPeaks(int bv_size, int threshold);

    /// Get bit vector size
    size_t get_bv_size() { return _bv_size; }

    /// Evaluate a bit vector
    double eval(const bit_vector_t&);

    /** Check for a known maximum.
     *
     * \return true
     */
    bool has_known_maximum() { return true; }

    /// Get the global maximum
    double get_maximum() { return _maximum; }

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

      De Bonet, J. S., Isbell, C. L., & Viola, P. (1997). MIMIC:
      Finding optima by estimating probability densities. Advances in
      neural information processing systems, 424-430.
  */
  class SixPeaks:
    public Function {

    /// Bit vector size
    size_t _bv_size;

    /// Threshold
    int _threshold;

    /// Maximum
    int _maximum;

  public:

    /// Constructor
    SixPeaks(int bv_size, int threshold);

    /// Get bit vector size
    size_t get_bv_size() { return _bv_size; }

    /// Evaluate a bit vector
    double eval(const bit_vector_t&);

    /** Check for a known maximum.
     *
     * \return true
     */
    bool has_known_maximum() { return true; }

    /// Get the global maximum
    double get_maximum() { return _maximum; }

  };


} // end of namespace function
} // end of namespace hnco


#endif
