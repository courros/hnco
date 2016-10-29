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

#ifndef HNCO_FUNCTIONS_FOUR_PEAKS_H
#define HNCO_FUNCTIONS_FOUR_PEAKS_H

#include "hnco/exception.hh"

#include "function.hh"


namespace hnco {
namespace function {


  /** Four Peaks function.

      It is defined by \f$f(x) = \max\{o(x), z(x)\} + r(x)\f$ where:
      - o(x) is the length of the longest prefix of x made of ones;
      - z(x) is the length of the longest suffix of x made of zeros;
      - r(x) is the reward equal to n if o(x) > t and z(x) > t, 0 otherwise.

      The threshold t is a parameter of the function.

      This function has four maxima, of which only two are global
      ones.

      For example, if n = 6 and t = s:
      - f(111111) = 6 (local maximum)
      - f(111110) = 5
      - f(111100) = 10 (global maximum)

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


} // end of namespace function
} // end of namespace hnco


#endif
