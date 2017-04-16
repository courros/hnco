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

#ifndef HNCO_FUNCTIONS_CANCELLATION_H
#define HNCO_FUNCTIONS_CANCELLATION_H

#include <vector>

#include "hnco/exception.hh"

#include "function.hh"


namespace hnco::function {

  /** Summation cancellation.

      Encoding of a signed integer:
      - bit 0: sign
      - bits 1 to 8: two's complement representation
  */
  class Cancellation:
    public Function {

  protected:

    /// Bit vector size
    size_t _bv_size;

    /// Buffer
    std::vector<double> _buffer;

    /// Convert a bit vector into a real vector
    void convert(const bit_vector_t& x);

  public:

    /** Constructor.

        The bit vector size n must be a multiple of 9. The size of
        _buffer is then n / 9.

        \param n Size of the bit vector
    */
    Cancellation(int n):
      _bv_size(n),
      _buffer(n / 9)
    {
      if (n % 9 != 0)
        throw exception::Error("Cancellation::Cancellation: _bv_size must be a multiple of 9");
    }

    /// Get bit vector size
    size_t get_bv_size() { return _bv_size; }

    /// Evaluate a bit vector
    double eval(const bit_vector_t& x);

    /** Check for a known maximum.
        \return true */
    bool has_known_maximum() { return true; }

    /// Get the global maximum
    double get_maximum() { return 0; }

  };


  /// Summation cancellation with sinus
  class SinusCancellation:
    public Cancellation {

  public:

    ///Constructor
    SinusCancellation(int n):
      Cancellation(n) {}

    /// Evaluate a bit vector
    double eval(const bit_vector_t& x);

  };

} // end of namespace hnco::function


#endif
