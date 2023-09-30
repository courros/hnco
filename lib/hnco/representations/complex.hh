/* Copyright (C) 2016, 2017, 2018, 2019, 2020, 2021, 2022, 2023 Arnaud Berny

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

#ifndef HNCO_REPRESENTATIONS_COMPLEX_H
#define HNCO_REPRESENTATIONS_COMPLEX_H

#include <assert.h>

#include <iostream>             // std::ostream
#include <complex>              // std::complex

#include "hnco/util.hh"         // hnco::is_in_range
#include "hnco/bit-vector.hh"


namespace hnco {
namespace representation {


/// Complex representation
template<class ScalarRep>
class ComplexRepresentation {

public:

  /// Scalar representation
  using scalar_rep = ScalarRep;

  /// Scalar type
  using scalar_type = typename scalar_rep::domain_type;

  /// Domain type
  using domain_type = std::complex<scalar_type>;

private:

  /// Representation of the real part
  scalar_rep _real_part;

  /// Representation of the imaginary part
  scalar_rep _imaginary_part;

public:

  /**
   * Constructor.
   *
   * @param real_part Representation of real part
   * @param imaginary_part Representation of imaginary part
   */
  ComplexRepresentation(scalar_rep real_part, scalar_rep imaginary_part)
    : _real_part(real_part)
    , _imaginary_part(imaginary_part)
  {}

  /**
   * Constructor.
   *
   * @param rep Representation of both real and imaginary parts
   */
  ComplexRepresentation(scalar_rep rep)
    : ComplexRepresentation(rep, rep)
  {}

  /// Size of the representation
  int size() const { return _real_part.size() + _imaginary_part.size(); }

  /// Unpack bit vector into a value
  domain_type unpack(const bit_vector_t& bv, int start) {
    assert(hnco::is_in_range(start, bv.size()));
    scalar_type re = _real_part.unpack(bv, start);
    start += _real_part.size();
    scalar_type im = _imaginary_part.unpack(bv, start);
    return domain_type(re, im);
  }

  /// Display
  void display(std::ostream& stream) const {
    stream << "ComplexRepresentation = ";
    _real_part.display(stream);
    stream << ", ";
    _imaginary_part.display(stream);
  }

};


} // end of namespace representation
} // end of namespace hnco


#endif
