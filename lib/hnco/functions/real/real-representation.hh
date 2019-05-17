/* Copyright (C) 2016, 2017, 2018, 2019 Arnaud Berny

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

#ifndef HNCO_FUNCTIONS_REAL_REAL_REPRESENTATION_H
#define HNCO_FUNCTIONS_REAL_REAL_REPRESENTATION_H

#include <assert.h>

#include "hnco/bit-vector.hh"


namespace hnco {
namespace function {
namespace real {

  /// Real representation
  class RealRepresentation {

  public:

    /// Destructor
    virtual ~RealRepresentation() {}

    /// Convert a bit vector range into a double
    virtual double convert(hnco::bit_vector_t::const_iterator first, hnco::bit_vector_t::const_iterator last) = 0;

  };


  /// Dyadic real representation
  class DyadicRealRepresentation {

    /// Lower bound
    double _lower_bound;

    /// Upper bound
    double _upper_bound;

    /// Number of bits
    int _num_bits;

  public:

    /// Constructor
    DyadicRealRepresentation(double lower_bound, double upper_bound, int num_bits):
      _lower_bound(lower_bound),
      _upper_bound(upper_bound),
      _num_bits(num_bits)
    {
      assert(_lower_bound < _upper_bound);
      assert(_num_bits > 0);
    }

    /// Convert a bit vector range into a double
    double convert(hnco::bit_vector_t::const_iterator first, hnco::bit_vector_t::const_iterator last);

  };


} // end of namespace real
} // end of namespace function
} // end of namespace hnco


#endif
