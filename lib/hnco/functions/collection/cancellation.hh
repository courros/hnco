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

#ifndef HNCO_FUNCTIONS_COLLECTION_CANCELLATION_H
#define HNCO_FUNCTIONS_COLLECTION_CANCELLATION_H

#include <vector>

#include "hnco/exception.hh"

#include "hnco/functions/function.hh"


namespace hnco {
namespace function {

  /** Summation cancellation.

      Encoding of a signed integer:
      - bit 0: sign
      - bits 1 to 8: two's complement representation

      Reference:

      S. Baluja and S. Davies. 1997. Using optimal dependency-trees
      for combinatorial optimization: learning the structure of the
      search space. Technical Report CMU- CS-97-107. Carnegie-Mellon
      University.

  */
  class SummationCancellation:
    public Function {

  protected:

    /// Bit vector size
    int _bv_size;

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
    SummationCancellation(int n):
      _bv_size(n),
      _buffer(n / 9)
    {
      if (n % 9 != 0)
        throw std::runtime_error("SummationCancellation::SummationCancellation: _bv_size must be a multiple of 9");
    }

    /// Get bit vector size
    int get_bv_size() const { return _bv_size; }

    /// Evaluate a bit vector
    double evaluate(const bit_vector_t& x);

    /** Check for a known maximum.
        \return true */
    bool has_known_maximum() const { return true; }

    /// Get the global maximum
    double get_maximum() const { return 0; }

  };


  /** Summation cancellation with sinus.

      Reference:

      M. Sebag and M. Schoenauer. 1997. A society of hill-climbers. In
      Proc. IEEE Int.  Conf. on Evolutionary
      Computation. Indianapolis, 319–324.

  */
  class SinusSummationCancellation:
    public SummationCancellation {

  public:

    ///Constructor
    SinusSummationCancellation(int n):
      SummationCancellation(n) {}

    /// Evaluate a bit vector
    double evaluate(const bit_vector_t& x);

  };

} // end of namespace function
} // end of namespace hnco


#endif
