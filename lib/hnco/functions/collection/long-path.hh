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

#ifndef HNCO_FUNCTIONS_COLLECTION_LONG_PATH_H
#define HNCO_FUNCTIONS_COLLECTION_LONG_PATH_H

#include "hnco/functions/function.hh"


namespace hnco {
namespace function {


/** Long path.

    Long paths have been introduced by Jeffrey Horn, David
    E. Goldberg, and Kalyanmoy Deb. Here we mostly follow the
    definition given by Thomas Jansen (see references below).

    As an example, here is the 2-long path of dimension 4:
    - 0000
    - 0001
    - 0011
    - 0111
    - 1111
    - 1101
    - 1100

    The fitness is increasing along the path. The fitness on the
    complementary of the path is defined as a linear function
    pointing to the beginning of the path.

    To help with the detection of maximum, we have dropped the
    constant \f$n^2\f$ whose sole purpose was to make the function
    non negative.

    References:

    Jeffrey Horn, David E. Goldberg, and Kalyanmoy Deb, "Long Path
    Problems", PPSN III, 1994.

    Thomas Jansen, Analyzing Evolutionary Algorithms. Springer, 2013.

*/
class LongPath:
    public Function {

  /// Bit vector size
  int _bv_size;

  /// Prefix length
  int _prefix_length;

public:

  /// Constructor
  LongPath(int bv_size, int prefix_length);

  /// Evaluate a bit vector
  double evaluate(const bit_vector_t&);

  /** @name Information about the function
   */
  ///@{

  /// Get bit vector size
  int get_bv_size() const { return _bv_size; }

  /** Check for a known maximum.

      Let \f$n\f$ be the bit vector size and \f$k\f$ the prefix
      length which must divide \f$n\f$.

      We have to check that the maximum can be represented exactly
      as a double, that is, it must be lower or equal to
      \f$2^{53}\f$. We are a little bit more conservative with the
      following test.

      If \f$\log_2(k) + n / k \le 53\f$ then returns true else
      returns false.

  */
  bool has_known_maximum() const;

  /** Get the global maximum.

      Let \f$n\f$ be the bit vector size and \f$k\f$ the prefix
      length which must divide \f$n\f$. Then the maximum is \f$k
      2^{n/k} - k + 1\f$.

      \throw std::runtime_error */
  double get_maximum() const;

  ///@}

};


} // end of namespace function
} // end of namespace hnco


#endif
