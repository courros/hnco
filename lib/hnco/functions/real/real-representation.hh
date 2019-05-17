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

#include <vector>

#include "hnco/bit-vector.hh"


namespace hnco {
namespace function {
namespace real {


  /// Real representation
  class RealRepresentation {

  public:

    /// Destructor
    virtual ~RealRepresentation() {}

    /// Size of the representation
    virtual int size() = 0;

    /// Convert a bit vector range into a double
    virtual double convert(hnco::bit_vector_t::const_iterator first, hnco::bit_vector_t::const_iterator last) = 0;

  };


  /// Dyadic real representation
  class DyadicRealRepresentation:
    public RealRepresentation {

    /// Lengths of dyadic intervals
    std::vector<double> _lengths;

    /// Lower bound of the search interval
    double _lower_bound;

    /// Length of the search interval
    double _length;

    /// Affine transformation
    double affine_transformation(double x) { return _lower_bound + _length * x; }

  public:

    /** Constructor.

        \param lower_bound Lower bound of the search interval
        \param upper_bound Upper bound of the search interval
        \param num_bits Number of bits per real
    */
    DyadicRealRepresentation(double lower_bound, double upper_bound, int num_bits);

    /// Size of the representation
    int size() { return _lengths.size(); }

    /// Convert a bit vector range into a double
    double convert(hnco::bit_vector_t::const_iterator first, hnco::bit_vector_t::const_iterator last);

  };


} // end of namespace real
} // end of namespace function
} // end of namespace hnco


#endif
