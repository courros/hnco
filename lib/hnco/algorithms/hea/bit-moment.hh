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

#ifndef HNCO_ALGORITHMS_HEA_MOMENT_BINARY_H
#define HNCO_ALGORITHMS_HEA_MOMENT_BINARY_H

#include <math.h>		// sqrt

#include <vector>
#include <iostream>

#include "hnco/bit-vector.hh"


namespace hnco {
namespace algorithm {
namespace hea {


  /// Moment for bit features
  struct BitMoment
  {

    /// Moment
    std::vector<std::vector<double> > _moment;

    /// Weight of second order moments
    double _weight = 1;

    /// Constructor
    BitMoment(int n):
      _moment(n, std::vector<double>(n, 0)) {}

    /// Set the moment to that of the uniform distribution
    void uniform();

    /// Initialize
    void init();

    /// Accumulate a bit vector
    void add(const bit_vector_t& x);

    /// Compute average
    void average(int count);

    /// Update moment
    void update(const BitMoment& p, double rate);

    /// Bound moment
    void bound(double margin);

    /// Distance
    double distance(const BitMoment& p) const;

    /// Compute the norm 2
    double norm_2() const;

    /// Compute the diameter
    double diameter() const {
      int n = _moment.size();
      return sqrt(n + _weight * (n * (n - 1) / 2));
    }

    /// Size
    size_t size() const { return _moment.size(); }

    /// Display
    void display(std::ostream& stream);

  };


}
}
}


#endif
