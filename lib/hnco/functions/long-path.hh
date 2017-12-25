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

#ifndef HNCO_FUNCTIONS_LONG_PATH_H
#define HNCO_FUNCTIONS_LONG_PATH_H

#include "function.hh"


namespace hnco {
namespace function {


  /** Long path.

      Long paths have been introduced by Jeffrey Horn, David
      E. Goldberg, and Kalyanmoy Deb. Here we follow the definition
      given by Thomas Jansen (see references below).

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

      References:

      Jeffrey Horn, David E. Goldberg, and Kalyanmoy Deb, "Long Path
      Problems", PPSN III, 1994.

      Thomas Jansen. 2013. Analyzing Evolutionary Algorithms. Springer.

  */
  class LongPath:
    public Function {

    /// Bit vector size
    size_t _bv_size;

    /// Prefix length
    int _prefix_length;

  public:

    /// Constructor
    LongPath(int bv_size, int prefix_length);

    /// Get bit vector size
    size_t get_bv_size() { return _bv_size; }

    /// Evaluate a bit vector
    double eval(const bit_vector_t&);

  };


} // end of namespace function
} // end of namespace hnco


#endif
