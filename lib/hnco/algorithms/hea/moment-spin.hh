/* Copyright (C) 2016, 2017, 2018 Arnaud Berny

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

#ifndef HNCO_ALGORITHMS_HEA_MOMENT_SPIN_H
#define HNCO_ALGORITHMS_HEA_MOMENT_SPIN_H

#include <vector>

#include "hnco/bit-vector.hh"


namespace hnco {
namespace algorithm {
namespace hea {


  /// Moment for spin variables
  struct SpinMoment
  {

    /// First moment
    std::vector<double> _first;

    /// Second moment
    std::vector<std::vector<double> > _second;

    /// Constructor
    SpinMoment(int n):
      _first(n, 0),
      _second(n, std::vector<double>(n, 0)) {}

    /// Weight of second order moments
    double _weight = 1;

    /// Set the moment to that of the uniform distribution
    void uniform();

    /// Initialize accumulators
    void init();

    /// Update accumulators
    void add(const bit_vector_t& x);

    /// Compute average
    void average(int count);

    /// Update moment
    void update(const SpinMoment& p, double rate);

    /// Bound moment
    void bound(double margin);

    /// Distance
    double distance(const SpinMoment& p) const;

    /// Compute the norm 2
    double norm_2() const;

    /// Compute the diameter
    double diameter() const {
      int n = _first.size();
      return 2 * sqrt(n + _weight * (n * (n - 1) / 2));
    }

    /// Size
    size_t size() const { return _first.size(); }

  };


}
}
}


#endif
