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

#ifndef HNCO_FUNCTIONS_LABS_H
#define HNCO_FUNCTIONS_LABS_H

#include <vector>

#include "function.hh"


namespace hnco {
namespace function {


  /// Low Autocorrelation Binary Sequences
  class Labs:
    public Function {

    /// Binary sequence written using 1 and -1
    std::vector<int> _sequence;

  public:

    /// Constructor
    Labs(int n):
      _sequence(n) {}

    /// Get bit vector size
    size_t get_bv_size() { return _sequence.size(); }

    /// Evaluate a bit vector
    double eval(const bit_vector_t&);

  };


} // end of namespace function
} // end of namespace hnco


#endif
